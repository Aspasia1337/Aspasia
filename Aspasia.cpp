#define IMGUI_DEFINE_MATH_OPERATORS

#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <type_traits>

#include "Helper/helper.h"
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

    bool ManualAA = 0;


    const char *MaterialNames[6] = { "WhiteMaterial", "Illuminate", "Latex", "Metalic" , "Glow", "Blom"};
    const char *BoneNames[7] = { "Head", "Torso", "Right Hand", "Left Hand" , "Pelvis", "Right Feet", "Left Feet"};

    int MaterialSelected = 0;
    int MaterialGunSelected = 0;
    int MaterialArmSelected = 0;

    int BoneSelected = 6;

    bool PlayersChams = 0;
    bool WeaponChams = 0;
    bool ArmsChams = 0;

    bool PVS = 0;

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
    unsigned char lightType = 0x02;

    bool Chams = 0;
    float ChamsColor [3] = { 0.5f, 1.5f, 1.0f };
    float ChamsColorGun[3] = { 1.5f, 0.5f, 1.0f };
    float ArmsColor[3] = { 0.5f, 0.5f, 1.3f };
    float InvisibleChamsColor[3] = { 0.3f,0.1f,0.3f };

    void *MouseEvent = nullptr;

    std::vector <Feature> Features = {
        Feature (&bhop,"BunnyHop"),
        Feature (&PlayersChams,"Player Chams"),
        Feature (&WeaponChams,"Weapon Chams"),
        Feature (&AntiAim,"AntiAim"),
        Feature (&RemoveScope,"RemoveScope"),
        Feature(&ChangeSkyColor,"Sky Changer"),
        Feature (&worldModulation, "World Modulation"),
        Feature (&lightModulation, "Light Modulation"),
        Feature (&RenderFlashHook, "Render Flash"),
        Feature (&RenderSmokeHook, "Render Smoke"),
        Feature (&RenderSkeleton, "Render Skeleton"),
        Feature (&RenderPlayerBones, "Render Bone ID"),
        Feature (&HealthBar, "Render Healt Bar"),
        Feature (&PlayerInfo, "Render Info"),
        Feature (&ShowSpectators, "Show Spectators"),
    };

    bool RenderPlayerBones = false;
    bool RenderSkeleton = false;

    std::unordered_map<const char *, terroristBones::tBones> boneMap = {
        {"Head", terroristBones::head_0},
        {"Neck", terroristBones::neck_0},
        {"Upper Chest", terroristBones::spine_2},
        {"Torso", terroristBones::spine_3},
        {"Pelvis", terroristBones::pelvis},

        {"Left Clavicle", terroristBones::clavicle_l},
        {"Left Upper Arm", terroristBones::arm_upper_l},
        {"Left Lower Arm", terroristBones::arm_lower_l},
        {"Left Hand", terroristBones::hand_l},

        {"Right Clavicle", terroristBones::clavicle_r},
        {"Right Upper Arm", terroristBones::arm_upper_r},
        {"Right Lower Arm", terroristBones::arm_lower_r},
        {"Right Hand", terroristBones::hand_r},

        {"Left Upper Leg", terroristBones::leg_upper_l},
        {"Left Lower Leg", terroristBones::leg_lower_l},
        {"Left Feet", terroristBones::ankle_l},

        {"Right Upper Leg", terroristBones::leg_upper_r},
        {"Right Lower Leg", terroristBones::leg_lower_r},
        {"Right Feet", terroristBones::ankle_r}
    };

    bool RemoveScope = 0;
    bool PlayerInfo = 0;
    bool OnlyEnemyRender = 0;
    bool InvisibleChams = 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
        if (threadHandle)
            CloseHandle(threadHandle);
    }
    return TRUE;
}


