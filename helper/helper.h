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

		void printTime(void);
		
	

		template <typename ...Args>
		void printMessage(messageMode mode, Args ...args ){

			Helper::Console::printTime();

			switch (mode) {
			case 0:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case 1:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case 2:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				break;
			};

			if constexpr (sizeof ...(args) > 0) {
				((std::cout << args<< " "), ...);
			}
			std::cout << std::endl;
			SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}



	};
	Console m_Console; // member console

	class Memory {
	public:

		std::vector<std::uint32_t> PatternToBytes(const char* pattern);
		std::uint8_t* PatternScanner(const char* moduleName, const char* signature);
		std::uint8_t *ResolveRip (std::uint8_t *address, std::uint32_t rvaOffset, std::uint32_t ripOffset);
	};
	Memory m_Mem;
};

inline Helper* helper = new Helper();

