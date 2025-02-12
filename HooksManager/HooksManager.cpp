#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "HooksManager.h"

#include "../helper/helper.h"
#include "../includes/kiero/minhook/include/MinHook.h"
#include "../globals.h"


HooksManager::SmokeEffect::RenderSmokeParticlesFunction HooksManager::SmokeEffect::oRenderSmokeParticles = nullptr;
HooksManager::FlashEffect::FlashEffectFunction HooksManager::FlashEffect::oFlashEffect = nullptr;
HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;

bool HooksManager::initHook ( ) {
	MH_STATUS hookInit;


	MH_Initialize ( );

	uint8_t *smokeEffectAddress = helper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B 9C 24 ? ? ? ? 4D 8B F8 ");
	
	if (smokeEffectAddress == nullptr)
		return false;

	hookInit = MH_CreateHook (
		smokeEffectAddress,
		reinterpret_cast<LPVOID *>(hookMgr->m_SmokeEffect.hRenderSmoke), 
		reinterpret_cast<LPVOID *>(&hookMgr->m_SmokeEffect.oRenderSmokeParticles));
	
	if(hookInit==MH_CREATE)
	helper->m_Console.printMessage (DEBUG, "SmokeEffectHook HOOKED! ");


	uint8_t *flashEffect = helper->m_Mem.PatternScanner ("client.dll", "57 48 83 EC 20 49 8B D8 48 8B F9 E8 ? ? ? ? 48 85 DB");

	hookInit = MH_CreateHook (
		flashEffect,
		reinterpret_cast<LPVOID *>(hookMgr->m_FlashEffect.hFlashEffect),
		reinterpret_cast<LPVOID *>(&hookMgr->m_FlashEffect.oFlashEffect));

	if (hookInit == MH_CREATE)
		helper->m_Console.printMessage (DEBUG, "FlashEffectHook HOOKED! ");


	uint8_t *createMove = helper->m_Mem.PatternScanner ("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18");

	hookInit = MH_CreateHook (
		createMove,
		reinterpret_cast<LPVOID *>(hookMgr->m_CreateMove.hCreateMove),
		reinterpret_cast<LPVOID *>(&hookMgr->m_CreateMove.oCreateMove));

	if (hookInit == MH_CREATE)
		helper->m_Console.printMessage (DEBUG, "Create Move HOOKED! ");


	MH_EnableHook (MH_ALL_HOOKS);
	return hookInit;
}



void HooksManager::SmokeEffect::hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6) {
	
	if (globals::RenderSmokeHook) {
		return;
	}

	return (hookMgr->m_SmokeEffect.oRenderSmokeParticles (a1,  a2,  a3,  a4,  a5,  a6));

}

void HooksManager::FlashEffect::hFlashEffect (__int64 a1, __int64 a2, float* a3) {

	if (globals::RenderFlashHook) {
		return;
	}

	return (hookMgr->m_FlashEffect.oFlashEffect (a1, a2, a3));
}

void HooksManager::CreateMove::hCreateMove (__int64 *a1, __int64 a2, float *a3) {
	


	return oCreateMove (a1, a2, a3);
}