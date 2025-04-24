#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>

using f_LoadLibraryA = HINSTANCE (WINAPI *)(const char *lpLibFilename);
using f_GetProcAddress = HINSTANCE (WINAPI *)(HINSTANCE hModule, const char *lpProcName);
using f_DLL_ENTRY_POINT = BOOL (WINAPI *)(void *hDll, DWORD dwReasion, void *pReserved);

struct MANUAL_MAPPING_DATA {
	f_LoadLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
	HINSTANCE hMod;
};

bool ManualMap (HANDLE hProc, const char *szDllFile);