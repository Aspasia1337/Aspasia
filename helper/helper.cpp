#include <Windows.h>
#include <ctime>
#include <stdio.h>

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

void Helper::Console::printMessage(const char* message, messageMode mode) {
	
	Helper::Console::printTime();

	switch (mode) {
	case 0:
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("%s\n", message);
		break;
	case 1:
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("%s\n", message);
		break;
	case 2:
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("%s\n", message);
		break;
	};

	SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}



Helper::Console::Console() {
	AllocConsole();

	SetConsoleTitle("m_Console");

	freopen_s(&m_ConsoleFile, "CONOUT$", "w", stdout);

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printMessage("Console allocated", INFO);
}


Helper::Console::~Console()
{
	fclose(m_ConsoleFile);
	FreeConsole();
}