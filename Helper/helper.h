#pragma once
#include "../Classes/Classes.h"

class CInterfaceRegister;

enum messageMode {
	DEBUG = 0,
	INFO,
	WARNING
};

class Helper
{
public:
	class Console {
	public:
		FILE *m_ConsoleFile;
		HANDLE m_hConsole;
		Console ( );
		~Console ( );
		void printTime (void);
		template <typename ...Args>
		void printMessage (messageMode mode, Args ...args) {
			Helper::Console::printTime ( );
			switch (mode) {
			case 0:
				SetConsoleTextAttribute (m_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case 1:
				SetConsoleTextAttribute (m_hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case 2:
				SetConsoleTextAttribute (m_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				break;
			};
			if constexpr (sizeof ...(args) > 0) {
				((std::cout << args << " "), ...);
			}
			std::cout << std::endl;
			SetConsoleTextAttribute (m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	};
	Console m_Console; // member console

	class Memory {
	public:
		// Convert pattern string to byte array
		std::vector<std::uint32_t> PatternToBytes (const char *pattern);
		// Scan module memory for byte pattern
		std::uint8_t *PatternScanner (const char *moduleName, const char *signature);
		// Resolve RIP-relative address
		std::uint8_t *ResolveRip (std::uint8_t *address, std::uint32_t rvaOffset, std::uint32_t ripOffset);
		template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
		T CallVMT (CBaseClass *thisptr, Args_t... argList)
		{
			using VirtualFn_t = T (__thiscall *)(const void *, decltype(argList)...);
			return (*reinterpret_cast<VirtualFn_t *const *>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex] (thisptr, argList...);
		}
		template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
		void PrintVMT (CBaseClass *thisptr, Args_t... argList)
		{
			using VirtualFn_t = T (__thiscall *)(const void *, decltype(argList)...);
			return (*reinterpret_cast<VirtualFn_t *const *>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex] (thisptr, argList...);
		}
		void *GetVMT (void *pointer, std::uint32_t index)
		{
			void **vtable = *static_cast<void ***>(pointer);
			return vtable[index];
		}
		template <typename T = std::uint8_t*>
		T *GetAbsoluteAddress (T *pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
		{
			pRelativeAddress += nPreOffset;
			pRelativeAddress += sizeof (std::int32_t) + *reinterpret_cast<std::int32_t *>(pRelativeAddress);
			pRelativeAddress += nPostOffset;
			return pRelativeAddress;
		}
		// Resolve relative address
		std::uint8_t *ResolveRelativeAddress (std::uint8_t *nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
		{
			std::uint32_t nRVA = *reinterpret_cast<std::uint32_t *>(nAddressBytes + nRVAOffset);
			std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;
			return reinterpret_cast<std::uint8_t *>(nRVA + nRIP);
		}
		// Get interface register list
		CInterfaceRegister *GetRegisterList (const wchar_t *wszModuleName);
	};
	Memory m_Mem;
};

inline Helper *iHelper = new Helper ( );