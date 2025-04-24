#include "injection.h"

bool ManualMap (HANDLE hProc, const char *szDllFile) {
	BYTE *pSrcData = nullptr;
	IMAGE_NT_HEADERS *pOldNtHeader = nullptr;
	IMAGE_OPTIONAL_HEADER *pOldOptHeader = nullptr;
	IMAGE_FILE_HEADER *pOldFileHeader = nullptr;
	BYTE *pTargetBase = nullptr;

	DWORD dwCheck = 0;
	if (!GetFileAttributesA (szDllFile)) {
		std::cout << "File is not Valid or doesn't exists\n";
		return 0;
	}

	std::ifstream File (szDllFile, std::ios::binary | std::ios::ate);

	if (File.fail ( )) {
		std::cout << "Cannot open the file\n";
		return 0;
	}

	auto FileSize = File.tellg ( );
	if (FileSize < 0x1000) { // Less than 4096 bytes
		std::cout << "Filesize is invalid \n";
		File.close ( );
		return 0;
	}

	//It's really important to do a static cast; this way we calculate if we can allocate
	//	at compile time.
	pSrcData = new BYTE[static_cast<UINT_PTR>(FileSize)];

	if (!pSrcData) {
		std::cout << "Memory allocation for the DLL failed\n";
		File.close ( );
		return 0;
	}


	File.seekg (0, std::ios::beg);
	File.read (reinterpret_cast<char *>(pSrcData), FileSize);
	File.close ( );

	//Check if it's a valid FILE
	if (reinterpret_cast<IMAGE_DOS_HEADER *>(pSrcData)->e_magic == 0x5A4D) { //MZ
		std::cout << "File isn't a valid PE File";
		delete[] pSrcData;
		return 0;
	}

	pOldNtHeader = reinterpret_cast<IMAGE_NT_HEADERS *>(pSrcData + reinterpret_cast<IMAGE_DOS_HEADER *>(pSrcData)->e_lfanew);
	pOldOptHeader = &pOldNtHeader->OptionalHeader;
	pOldFileHeader = &pOldNtHeader->FileHeader;

	if (pOldFileHeader->Machine != IMAGE_FILE_MACHINE_AMD64) {
		std::cout << "Invalid build platform from the DLL\n";
		delete[] pSrcData;
		return 0;
	}

	// Try to allocate it and set in the place he preffers
	pTargetBase = reinterpret_cast<BYTE *>(VirtualAllocEx (hProc, reinterpret_cast<void *>(pOldOptHeader->ImageBase), pOldOptHeader->SizeOfImage, 
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	if (!pTargetBase) {
		std::cout << "Couldn't allocate on the preffered place, trying at any free place \n";

		pTargetBase = reinterpret_cast<BYTE *>(VirtualAllocEx (hProc,nullptr, pOldOptHeader->SizeOfImage,
			MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!pTargetBase) {
			std::cout << "Couldn't allocate in any place\n";
			delete[] pSrcData;
			return 0;
		}

	}

	MANUAL_MAPPING_DATA data{ 0 };
	data.pLoadLibraryA = LoadLibraryA;
	data.pGetProcAddress = reinterpret_cast<f_GetProcAddress>(GetProcAddress);


	//Let's map the sections

	auto *pSectionHeader = IMAGE_FIRST_SECTION (pOldNtHeader); //First section after the optional header
	for (uint64_t i = 0; i != pOldFileHeader->NumberOfSections; i++, pSectionHeader++) {

		if (pSectionHeader->SizeOfRawData) { //We only care of sections that have RawData in it (data wich is initialized)
			if (!WriteProcessMemory (hProc, pTargetBase + pSectionHeader->VirtualAddress, pSrcData + pSectionHeader->PointerToRawData, 
				pSectionHeader->SizeOfRawData, nullptr)) {
				std::cout << "Cannot map sections\n";
				delete[] pSrcData;
				//VirtualFreeEx (hProc, pTargetBase, MEM_RELEASE.);
				return 0;
			}
		}
	}
}