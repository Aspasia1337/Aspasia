#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "helper.h"

void Helper::Console::printTime(void)
{
	std::time_t now = std::time(nullptr);
	std::tm localTime;

	if (localtime_s(&localTime, &now) == 0)
	{
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("[%02d:%02d:%02d] - ",
			   localTime.tm_hour,
			   localTime.tm_min,
			   localTime.tm_sec);
	}
}



Helper::Console::Console()
{
	AllocConsole();

	SetConsoleTitle("m_Console");

	freopen_s(&m_ConsoleFile, "CONOUT$", "w", stdout);

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printMessage(DEBUG, "Aspasia ~ Debug Console");
}

Helper::Console::~Console()
{

	if (m_ConsoleFile)
	{
		fclose(m_ConsoleFile);
		m_ConsoleFile = nullptr;
	}

	freopen_s(&m_ConsoleFile,"CONOUT$","w",stdout);
	FreeConsole();

	HWND hConsole = GetConsoleWindow();
	if (hConsole)
	{
		PostMessage(hConsole, WM_CLOSE, 0, 0);
	}
}



std::vector<std::uint32_t> Helper::Memory::PatternToBytes(const char *pattern)
{
	std::vector<uint32_t> result;

	char *start = (char *)(pattern);

	do
	{
		if (*start == '?')
		{
			start += 1;
			if (*start == '?')
			{
				start += 1;
			}
			result.push_back(-1);
		}
		else
		{
			result.push_back(std::strtoul(start, &start, 16));
		}
		start += 1;

	} while (*start != '\0');

	return result;
}

std::uint8_t *Helper::Memory::PatternScanner(const char *module_name, const char *sig)
{

	const char *signature = sig;
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t *>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = PatternToBytes(signature);
	std::uint8_t *image_base = reinterpret_cast<std::uint8_t *>(module_handle);

	std::size_t pattern_size = pattern_bytes.size();
	std::uint32_t *array_of_bytes = pattern_bytes.data();

	for (std::size_t i = 0; i < size_of_image - pattern_size; i++)
	{
		bool found = true;

		for (std::size_t j = 0; j < pattern_size; j++)
		{
			if (image_base[i + j] != array_of_bytes[j] && array_of_bytes[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			// helper->m_Console.printMessage (INFO, "Pattern found");
			return &image_base[i];
		}
	}

	return nullptr;
}

std::uint8_t *Helper::Memory::ResolveRip(std::uint8_t *address, std::uint32_t rvaOffset, std::uint32_t ripOffset)
{
	if (!address || !rvaOffset || !ripOffset)
		return nullptr;

	// lea rcx, unk_1AA17C0
	// 48 8D 0D ? ? ? ?
	// rvaOffset = 3
	// ripOffset = 7

	uint32_t rva = *reinterpret_cast<uint32_t *>(address + rvaOffset);
	uint64_t rip = reinterpret_cast<uint64_t>(address) + ripOffset; // !!!! uint64_t

	return reinterpret_cast<uint8_t *>(rva + rip);
}

template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
constexpr int StringCompareN (const C *tszLeft, const C *tszRight, std::size_t nCount)
{
	using ComparisonType_t = std::conditional_t<std::is_same_v<C, char>, std::uint8_t, std::conditional_t<sizeof (wchar_t) == 2U, std::int16_t, std::int32_t>>;

	ComparisonType_t nLeft, nRight;
	while (nCount--)
	{
		nLeft = static_cast<ComparisonType_t>(*tszLeft++);
		nRight = static_cast<ComparisonType_t>(*tszRight++);

		if (nLeft != nRight)
			return nLeft - nRight;

		if (nLeft == C ('\0'))
			break;
	}

	return 0;
}

void *GetExportAddress (const void *hModuleBase, const char *szProcedureName)
{
	const auto pBaseAddress = static_cast<const std::uint8_t *>(hModuleBase);

	const auto pIDH = static_cast<const IMAGE_DOS_HEADER *>(hModuleBase);
	if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;

	const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS64 *>(pBaseAddress + pIDH->e_lfanew);
	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		return nullptr;

	const IMAGE_OPTIONAL_HEADER64 *pIOH = &pINH->OptionalHeader;
	const std::uintptr_t nExportDirectorySize = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	const std::uintptr_t uExportDirectoryAddress = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	if (nExportDirectorySize == 0U || uExportDirectoryAddress == 0U)
	{
		return nullptr;
	}

	const auto pIED = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY *>(pBaseAddress + uExportDirectoryAddress);
	const auto pNamesRVA = reinterpret_cast<const std::uint32_t *>(pBaseAddress + pIED->AddressOfNames);
	const auto pNameOrdinalsRVA = reinterpret_cast<const std::uint16_t *>(pBaseAddress + pIED->AddressOfNameOrdinals);
	const auto pFunctionsRVA = reinterpret_cast<const std::uint32_t *>(pBaseAddress + pIED->AddressOfFunctions);

	// Perform binary search to find the export by name
	std::size_t nRight = pIED->NumberOfNames, nLeft = 0U;
	while (nRight != nLeft)
	{
		// Avoid INT_MAX/2 overflow
		const std::size_t uMiddle = nLeft + ((nRight - nLeft) >> 1U);
		const int iResult = strcmp (szProcedureName, reinterpret_cast<const char *>(pBaseAddress + pNamesRVA[uMiddle]));

		if (iResult == 0)
		{
			const std::uint32_t uFunctionRVA = pFunctionsRVA[pNameOrdinalsRVA[uMiddle]];

			if (uFunctionRVA >= uExportDirectoryAddress && uFunctionRVA - uExportDirectoryAddress < nExportDirectorySize)
			{
				break;
			}

			return const_cast<std::uint8_t *>(pBaseAddress) + uFunctionRVA;
		}

		if (iResult > 0)
			nLeft = uMiddle + 1;
		else
			nRight = uMiddle;
	}

	return nullptr;
}

bool GetSectionInfo (const void *hModuleBase, const char *szSectionName, std::uint8_t **ppSectionStart, std::size_t *pnSectionSize)
{
	const auto pBaseAddress = static_cast<const std::uint8_t *>(hModuleBase);

	const auto pIDH = static_cast<const IMAGE_DOS_HEADER *>(hModuleBase);
	if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS *>(pBaseAddress + pIDH->e_lfanew);
	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		return false;

	const IMAGE_SECTION_HEADER *pISH = IMAGE_FIRST_SECTION (pINH);

	for (WORD i = 0U; i < pINH->FileHeader.NumberOfSections; i++, pISH++)
	{
		if (StringCompareN (szSectionName, reinterpret_cast<const char *>(pISH->Name), IMAGE_SIZEOF_SHORT_NAME) == 0)
		{
			if (ppSectionStart != nullptr)
				*ppSectionStart = const_cast<std::uint8_t *>(pBaseAddress) + pISH->VirtualAddress;

			if (pnSectionSize != nullptr)
				*pnSectionSize = pISH->SizeOfRawData;

			return true;
		}
	}

	return false;
}
