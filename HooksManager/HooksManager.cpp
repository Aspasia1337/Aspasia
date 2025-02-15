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

HooksManager::SmokeEffect::RenderSmokeParticlesFunction HooksManager::SmokeEffect::oRenderSmokeParticles = nullptr;
HooksManager::FlashEffect::FlashEffectFunction HooksManager::FlashEffect::oFlashEffect = nullptr;
HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;

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

	uint8_t *setViewAngles = iHelper->m_Mem.PatternScanner ("client.dll", "85 D2 75 3F 48");

	hookInit = MH_CreateHook (
		setViewAngles,
		reinterpret_cast<LPVOID *>(iHooksManager->m_SetViewAngles.hSetViewAngles),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_SetViewAngles.oSetViewAngles));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Set View Angles HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Set View Angles! ");

	MH_EnableHook (MH_ALL_HOOKS);
	return hookInit;
}



void HooksManager::SmokeEffect::hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6) {
	
	if (globals::RenderSmokeHook) {
		return;
	}

	return (oRenderSmokeParticles (a1,  a2,  a3,  a4,  a5,  a6));

}

void HooksManager::FlashEffect::hFlashEffect (__int64 a1, __int64 a2, float* a3) {

	if (globals::RenderFlashHook) {
		return;
	}

	return (oFlashEffect (a1, a2, a3));
}




void HooksManager::CreateMove::hCreateMove (CCSGOInput *a1, __int64 a2, __int64* a3) {
	
	if (globals::CreateMoveHook && GetAsyncKeyState(RI_MOUSE_LEFT_BUTTON_DOWN & 1)) {
		iGameEntitySystem->getAllPlayers ( );
		SetViewAngles::hSetViewAngles ((__int64*)a1, 0, CalculateAngles(iGameEntitySystem->LocalPlayerPawn->vOldOrigin, iGameEntitySystem->PawnVector[2]->vOldOrigin));
	}
	return oCreateMove (a1, a2, a3);
}



void HooksManager::SetViewAngles::hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3) {

	//if (globals::SetViewAngles) {
		//return oSetViewAngles (a1,0,);
	//}

	return oSetViewAngles (a1, a2, a3);

}

