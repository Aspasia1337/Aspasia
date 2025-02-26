#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "HooksManager.h"

#include "../helper/helper.h"
#include "../includes/kiero/minhook/include/MinHook.h"
#include "../globals.h"
#include "../math/vector.h"

#include "../EntityManager/EntityManager.h"
#include "../visuals/Visuals.h"

HooksManager::SmokeEffect::RenderSmokeParticlesFunction HooksManager::SmokeEffect::oRenderSmokeParticles = nullptr;
HooksManager::FlashEffect::FlashEffectFunction HooksManager::FlashEffect::oFlashEffect = nullptr;
HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::CreateMoveTWO::CreateMoveFunctionTWO HooksManager::CreateMoveTWO::oCreateMoveTWO = nullptr;

HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;
HooksManager::DrawObjectClass::DrawObjectFunction HooksManager::DrawObjectClass::oDrawObject = nullptr;

bool HooksManager::initHook ( ) {

	iHelper->m_Console.printMessage (INFO, "Hoosk Manager Init");

	MH_STATUS hookInit;

	MH_Initialize ( );

	uint8_t *smokeEffectAddress = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B 9C 24 ? ? ? ? 4D 8B F8 ");
	
	if (smokeEffectAddress == nullptr)
		return false;

	hookInit = MH_CreateHook (
		smokeEffectAddress,
		reinterpret_cast<LPVOID *>(iHooksManager->m_SmokeEffect.hRenderSmoke), 
		reinterpret_cast<LPVOID *>(&iHooksManager->m_SmokeEffect.oRenderSmokeParticles));
	
	if(hookInit==MH_OK)
	iHelper->m_Console.printMessage (DEBUG, "\t Smoke Effect HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING SmokeEffect! ");


	uint8_t *flashEffect = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 0F 29 74 24 ? 33 FF 0F");

	hookInit = MH_CreateHook (
		flashEffect,
		reinterpret_cast<LPVOID *>(iHooksManager->m_FlashEffect.hFlashEffect),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_FlashEffect.oFlashEffect));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Flash Effect HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Flash Effect! ");


	uint8_t *createMove = iHelper->m_Mem.PatternScanner ("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18");

	hookInit = MH_CreateHook (
		createMove,
		reinterpret_cast<LPVOID *>(iHooksManager->m_CreateMove.hCreateMove),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_CreateMove.oCreateMove));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Create Move HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Create Move! ");


	uint8_t *createMoveTWO = iHelper->m_Mem.PatternScanner ("client.dll", "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 57");

	hookInit = MH_CreateHook (
		createMoveTWO,
		reinterpret_cast<LPVOID *>(iHooksManager->m_CreateMoveTWO.hCreateMoveTWO),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_CreateMoveTWO.oCreateMoveTWO));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Create MoveTWO HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Create MoveTWO! ");


	uint8_t *setViewAngles = iHelper->m_Mem.PatternScanner ("client.dll", "85 D2 75 3F 48");

	hookInit = MH_CreateHook (
		setViewAngles,
		reinterpret_cast<LPVOID *>(iHooksManager->m_SetViewAngles.hSetViewAngles),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_SetViewAngles.oSetViewAngles));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Set View Angles HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Set View Angles! ");


	uint8_t *drawObjectaddrr = iHelper->m_Mem.PatternScanner ("scenesystem.dll", "48 8B C4 48 89 50 ? 53");

	hookInit = MH_CreateHook (
		drawObjectaddrr,
		reinterpret_cast<LPVOID *>(iHooksManager->m_DrawObject.hDrawObject),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_DrawObject.oDrawObject));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Draw Object HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Draw Object! ");


		int (__fastcall * CreateMaterialFunction)(void *, void *, const char *, void *, unsigned int, unsigned int);

	CreateMaterialFunction = reinterpret_cast<decltype(CreateMaterialFunction)>(iHelper->m_Mem.PatternScanner ("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05"));


	if (!CreateMaterialFunction) {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Create Material! ");
	}




	MH_EnableHook (MH_ALL_HOOKS);
	return hookInit;
}



void HooksManager::SmokeEffect::hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6) {
	
	if (globals::RenderSmokeHook) {
		return;
	}

	return (oRenderSmokeParticles (a1,  a2,  a3,  a4,  a5,  a6));

}

void HooksManager::DrawObjectClass::hDrawObject (void *a1, void *a2, void *a3, int a4, void *a5, void *a6, void *a7, void *a8) {

	return oDrawObject (a1, a2, a3, a4, a5,a6, a7, a8);
}

void HooksManager::FlashEffect::hFlashEffect (__int64 a1, __int64 a2, float* a3) {



	if (globals::RenderFlashHook) {

		return;
	}

	return (oFlashEffect (a1, a2, a3));
}

int IN_JUMP = (1 << 1);


void HooksManager::CreateMove::hCreateMove (CCSGOInput *a1, __int64 a2, __int64* a3) {
	float dist;

	if (globals::CreateMoveHook && GetAsyncKeyState (RI_MOUSE_LEFT_BUTTON_DOWN & 1) && iGameEntitySystem->PlayersVector[0]->Pawn->isInFov) {
		if (iGameEntitySystem->PlayersVector[0]->Pawn->pawnHealth > 0) {
			SetViewAngles::hSetViewAngles ((__int64 *)a1, 0, CalculateAngles (iGameEntitySystem->LocalPlayerPawn->vOldOrigin, iGameEntitySystem->PlayersVector[0]->Pawn->vOldOrigin, globals::aimbotFov));
		}
	}


	if (globals::glow) {
		iVisuals->glowPlayers (globals::glowType, globals::chamsColor);
	}


	return oCreateMove (a1, a2, a3);
}


// a2 = basecmd
void HooksManager::CreateMoveTWO::hCreateMoveTWO (CCSGOInput *a1, __int64 *a2, CUserCmd *a3) {

	oCreateMoveTWO (a1, a2, a3);

	if (globals::bhop) {
		if (iGameEntitySystem->LocalPlayerPawn->m_fFlags & 257 && a3->buttons & (1 << 1)) {
			a3->buttons &= ~(1 << 1);
		}
	}
}


void HooksManager::SetViewAngles::hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3) {



	return oSetViewAngles (a1, a2, a3);

}

