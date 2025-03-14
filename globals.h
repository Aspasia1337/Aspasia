#pragma once

#include "math/vector.h"
#include <vector>
#include <iostream>

namespace Globals {
	extern float chamsColor[4];
	extern int tab;
	extern int glowType;
	extern bool showMenu;

	extern bool glow;
	extern bool ShowInfo;
	extern bool noFlash;
	extern bool noSmoke;
	extern bool smokeVisuals;

	extern bool RenderSmokeHook;
	extern bool RenderFlashHook;
	extern bool CreateMoveHook;
	extern bool SetViewAngles;
	extern float smoothing;
	extern float maxFov;
	extern float aimbotFov;
	extern bool anti_aim;
	extern Vec3 viewAngles;
	extern bool lightModulation;
	extern bool worldModulation;
	extern bool bhop;
	extern bool AntiAim;
	extern bool thirdPerson;
	extern float fov1;
	extern float fov2;
	extern float fov3;

	extern float smokeColorRGB[3];
	extern float lightModulationColor[3];
	extern float worldModulationColor[3];
	extern byte lightType;
	extern float lightIntensity;

	extern std::vector<std::string> Spectators;

	extern float AAPitch;
	extern float AAJaw;
	extern bool ShowSpectators;
	extern bool ChangeSkyColor;
	extern float SkyModulationColor[3];
	extern float SkyTintColor[3];

	extern const char *MaterialNames[5];
	
	extern int  MaterialSelected;
	extern int MaterialGunSelected;


	extern bool PlayersChams;
	extern bool WeaponChams;

	extern uint32_t ping;

	extern bool Chams;
	extern float ChamsColor[3];
	extern float ChamsColorGun[3];
	extern float SkyTintColor[3];

};
