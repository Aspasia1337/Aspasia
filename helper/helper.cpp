#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>

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

	helper->m_Console.printMessage(WARNING, "Pattern: ", pattern, " found!");

	return result;
}


std::uint8_t* Helper::Memory::PatternScanner(const char* moduleName, const char* signature) {
	return 0;
}

