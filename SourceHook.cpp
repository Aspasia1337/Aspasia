#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "helper/helper.h"

void mainThread(HMODULE hModule) {

    while (1) {

    }

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, NULL);
        if (threadHandle)
            CloseHandle(threadHandle);
    }
    return TRUE;
}


