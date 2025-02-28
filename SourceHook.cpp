#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <type_traits>

#include "helper/helper.h"
#include "EntityManager/EntityManager.h"
#include "HooksManager/HooksManager.h"

#include "globals.h" 


#include "MainThread/mainHook.h"



namespace globals {
    float chamsColor[4] = { 1, 1, 1, 1 };
    int tab = 0;
    int glowType = 0;
    bool showMenu = false;

    bool glow = false;
    bool ShowInfo = false;
    bool noFlash = false;
    bool noSmoke = false;
    bool smokeVisuals = false;
    bool RenderSmokeHook = false;
    bool RenderFlashHook = false;
    bool CreateMoveHook = false;
    bool SetViewAngles = false;
    float smoothing = 1;
    float maxFov = 50.0f;
    float aimbotFov = 50;
    bool anti_aim = false;
    bool lightModulation = false;
    bool worldModulation = false;
    bool bhop = false;
    bool antiAim = false;
    bool thirdPerson = false;

    float fov1;
    float fov2;
    float fov3;

    Vec3 viewAngles;


    float smokeColorRGB[3] = { 0.0f, 0.0f, 0.0f }; // Inicializamos en negro
    float lightModulationColor[3] = { 1.0f, 1.0f, 1.0f };
    float worldModulationColor[3] = { 1.0f, 1.0f, 1.0f };

    float lightIntensity = 1;
    byte lightType = 0x02;
}

/*
DWORD WINAPI mainThread(HMODULE hModule) {


    bool glow = false;


    while (1) {
         if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
             glow = !glow;
         }
         if (GetAsyncKeyState(VK_INSERT) & 1)
         {

         }
         if (GetAsyncKeyState(VK_NUMPAD1) & 1)
         {
             break;

         }

         if (glow) {

             //cgame->glowPatch();
             cgame->getAllPlayers ( );
             cgame->renderDot ( );
         }
    }

    FreeLibraryAndExitThread(hModule,0);
    return 0;
}

*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
      //  HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, NULL
        if (threadHandle)
            CloseHandle(threadHandle);
    }
    return TRUE;
}


