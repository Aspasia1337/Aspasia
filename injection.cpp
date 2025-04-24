#include "injection.h"

void __stdcall ShellCode (MANUAL_MAPPING_DATA *pData);

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
	if (reinterpret_cast<IMAGE_DOS_HEADER *>(pSrcData)->e_magic == 0x5A4D) { //MZ -> PE Magic Number
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
				VirtualFreeEx (hProc, pTargetBase, 0, MEM_RELEASE,);
				return 0;
			}
		}
	}

	memcpy (pSrcData, &data, sizeof (data));
	WriteProcessMemory (hProc, pTargetBase, pSrcData, 0x1000, nullptr);

	delete[]pSrcData;

	void *pShellcode = VirtualAllocEx (hProc, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!pShellcode) {
		std::cout << "Shellcode memory allocation failed\n";
		VirtualFreeEx (hProc,pTargetBase, 0, MEM_RELEASE);
		return false;
	}

	WriteProcessMemory (hProc, pShellcode, Shellcode, 0x1000, nullptr);

	HANDLE hThread = CreateRemoteThread (hProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellcode), pTargetBase, 0, nullptr);

	if (!hThread) {
		std::cout << "Couldn't create the Thread\n";
		VirtualFreeEx (hProc, pTargetBase, 0, MEM_RELEASE);
		return false;
	}


	CloseHandle (hThread);

	HINSTANCE hCheck = NULL;

	while (!hCheck) {
		MANUAL_MAPPING_DATA data_checked{ 0 };
		ReadProcessMemory (hProc, pTargetBase, &data_checked, sizeof (data_checked), nullptr);
		hCheck = data_checked.hMod;
	}


	return true;
}

#define RELOC_FLAG(RelInfo)((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)


void __stdcall Shellcode (MANUAL_MAPPING_DATA *pData) {
	if (!pData)
		return;

	BYTE *pBase = reinterpret_cast<BYTE *>(pData);
	auto *pOptional = &reinterpret_cast<IMAGE_NT_HEADERS *>(pBase + reinterpret_cast<IMAGE_DOS_HEADER *>(pData)->e_lfanew)->OptionalHeader;

	auto _LoadLibraryA = pData->pLoadLibraryA;
	auto _GetProcaddress = pData->pGetProcAddress;
	auto _DLLMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOptional->AddressOfEntryPoint);

	BYTE *LocationDelta = pBase - pOptional->ImageBase;
	
	if (LocationDelta) {
		if (!pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
			return; //We cannot relocate the data
		}

		auto *pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION *>(pBase + pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocData->VirtualAddress) {

			UINT AmountOfEntries = (pRelocData->SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION)) / sizeof (WORD);
			WORD *pRelativeInfo = reinterpret_cast<WORD *>(pRelocData + 1);
			
			for (UINT i = 0; i != AmountOfEntries; i++, pRelativeInfo++) {
				if (RELOC_FLAG (*pRelativeInfo)) {
					UINT_PTR *pPatch = reinterpret_cast<UINT_PTR *>(pBase + pRelocData->VirtualAddress + ((*pRelativeInfo) & 0xFFF));
					*pPatch += reinterpret_cast<UINT_PTR>(LocationDelta);
				}
			}

			pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION *>(reinterpret_cast<BYTE *>(pRelocData) + pRelocData->SizeOfBlock);
		}
	}

	if (pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
		auto *pImportDescriptor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR *>(pBase + pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDescriptor->Name) {
			char *szMod = reinterpret_cast<char *>(pBase + pImportDescriptor->Name);
			HINSTANCE hDll = _LoadLibraryA (szMod);
			ULONG_PTR *pThunkRef = reinterpret_cast<ULONG_PTR *>(pBase + pImportDescriptor->OriginalFirstThunk);
			ULONG_PTR *pFunctRef = reinterpret_cast<ULONG_PTR *>(pBase + pImportDescriptor->FirstThunk);

			if (!pThunkRef) {
				pThunkRef = pFunctRef;
			}

			for (; *pThunkRef; pThunkRef++, pFunctRef++) {
				if (IMAGE_SNAP_BY_ORDINAL(*pThunkRef)) {
					*pFunctRef = _GetProcaddress (hDll, reinterpret_cast<char *>(*pThunkRef & 0xFFFF));
				}
				else {
					auto *pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME *>(pBase + (*pThunkRef));
					*pFunctRef = _GetProcaddress (hDll, pImport->Name);
				}
			}
			pImportDescriptor++;
		}
	}



	if (pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) {
		auto *pTls = reinterpret_cast<IMAGE_TLS_DIRECTORY *>(pBase + pOptional->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto *pCallback = reinterpret_cast<PIMAGE_TLS_CALLBACK *>(pTls->AddressOfCallBacks);
		for (; pCallback && *pCallback; pCallback++) {
			(*pCallback)(pBase, DLL_PROCESS_ATTACH, nullptr);
		}
	}

	_DLLMain (pBase, DLL_THREAD_ATTACH, nullptr);

	pData->hMod = reinterpret_cast<HINSTANCE>(pBase);
	
}