#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include "helper.h"

void Helper::Console::printTime(void) {
	std::time_t now = std::time(nullptr);
	std::tm localTime;

	if (localtime_s(&localTime, &now) == 0) {
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("[%02d:%02d:%02d] - ",
			localTime.tm_hour,
			localTime.tm_min,
			localTime.tm_sec);
	}
}


Helper::Console::Console() {
	AllocConsole();

	SetConsoleTitle("m_Console");

	freopen_s(&m_ConsoleFile, "CONOUT$", "w", stdout);

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printMessage(INFO, "Console allocated");
}


Helper::Console::~Console()
{
	fclose(m_ConsoleFile);
	FreeConsole();
}


std::vector<std::uint32_t> Helper::Memory::PatternToBytes(const char* pattern)
{
	std::vector<uint32_t> result;

	char* start = (char*)(pattern);

	do {
		if (*start == '?') {
			start += 1;
			if (*start == '?') {
				start += 1;
			}
			result.push_back(-1);
		}
		else {
			result.push_back(std::strtoul(start, &start, 16));
		}
		start += 1;

	} while (*start != '\0');

	return result;
}


Helper::Memory::Memory() {



}



std::uint8_t* Helper::Memory::PatternScanner(const char* module_name, const char* sig)
{

	const char* signature = sig;
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = PatternToBytes(signature);
	std::uint8_t* image_base = reinterpret_cast<std::uint8_t*>(module_handle);

	std::size_t pattern_size = pattern_bytes.size();
	std::uint32_t* array_of_bytes = pattern_bytes.data();

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
			helper->m_Console.printMessage(INFO, "Pattern found");
			return &image_base[i];
		}
	}

	return nullptr;
}


