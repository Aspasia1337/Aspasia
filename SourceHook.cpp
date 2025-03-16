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



namespace Globals {
    float chamsColor[4] = { 1, 1, 1, 1 };
    int tab = 0;
    int glowType = 0;
    bool showMenu = false;
    bool LastMode = true;

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
    bool AntiAim = false;
    bool thirdPerson = false;
    bool ChangeSkyColor = false;
    float SkyModulationColor [3] = {0, 0, 0.};
    float SkyTintColor[3] = { 0, 0, 0 };


    const char *MaterialNames[5] = { "WhiteMaterial", "Illuminate", "Latex", "Metalic" , "Glow" };
    int MaterialSelected = 0;
    int MaterialGunSelected = 0;

    bool PlayersChams = 0;
    bool WeaponChams = 0;

    bool HealthBar = 0;


    float fov1;
    float fov2;
    float fov3;

    Vec3 viewAngles;


    float smokeColorRGB[3] = { 0.0f, 0.0f, 0.0f }; 
    float lightModulationColor[3] = { 1.0f, 1.0f, 1.0f };
    float worldModulationColor[3] = { 1.0f, 1.0f, 1.0f };

    float AAPitch = 0;
    float AAJaw = 0;

    bool ShowSpectators = 0;
    std::vector<std::string> Spectators;

    uint32_t ping = 0;

    float lightIntensity = 1;
    byte lightType = 0x02;

    bool Chams = 0;
    float ChamsColor [3] = { 0.5f, 0.5f, 1.0f };
    float ChamsColorGun[3] = { 0.5f, 0.5f, 1.0f };

    void *MouseEvent = nullptr;

    std::vector <Feature> Features = {
        Feature (&bhop,"BunnyHop"),
        Feature (&PlayersChams,"Player Chams"),
        Feature (&WeaponChams,"Weapon Chams"),
        Feature (&AntiAim,"AntiAim"),
    };

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
      //  HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, NULL
        if (threadHandle)
            CloseHandle(threadHandle);
    }
    return TRUE;
}


