#pragma once

enum messageMode {
	DEBUG = 0,
	INFO,
	WARNING
} ;

class Helper
{
public:
	class Console {
	public:
		FILE* m_ConsoleFile;
		HANDLE m_hConsole;

		Console();
		~Console();

		void printMessage(const char* message, messageMode mode);
		void printTime(void);

	};
	Console m_Console; // member console



};

inline Helper* helper = new Helper();



