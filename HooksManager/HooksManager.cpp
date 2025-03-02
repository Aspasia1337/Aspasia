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
#include "../Interfaces/iManager.h"

#include "../Features/Anti-Aim/AntiAim.h"
#include "../Features/Movement/Movement.h"
#include "../Features/Visuals/Visual.h"

HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::CreateMoveTWO::CreateMoveFunctionTWO HooksManager::CreateMoveTWO::oCreateMoveTWO = nullptr;

HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;
HooksManager::WorldModulation::oModulateWorldColorFn HooksManager::WorldModulation::oModulateWorldColor = nullptr;
HooksManager::ValidateInput::ValidateInputFunction HooksManager::ValidateInput::oValidateInput = nullptr;
HooksManager::OverrideViewClass::OverrideViewFunction HooksManager::OverrideViewClass::oOverrideViewFunction = nullptr;


AntiAim::ValidateInput::ValidateInputFunction AntiAim::ValidateInput::oValidateInput = nullptr;
Visual::LightningModulation::LightningModulationFunction Visual::LightningModulation::oLightningModulation = nullptr;
Visual::SmokeRender::RenderSmokeParticlesFunction Visual::SmokeRender::oRenderSmokeParticles = nullptr;
Visual::FlashEffect::FlashEffectFunction Visual::FlashEffect::oFlashEffect = nullptr;
Visual::DrawObjectClass::DrawObjectFunction Visual::DrawObjectClass::oDrawObject = nullptr;


Vec3 AntiAim::PlayerAngles;

HooksManager::calcBonesFunction calcBones = nullptr;
Vec3 antiAimAngles;

bool HooksManager::initHook ( ) {

	iHelper->m_Console.printMessage (INFO, "Hoosk Manager Init");

	MH_STATUS hookInit;

	MH_Initialize ( );

	uint8_t *smokeEffectAddress = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B 9C 24 ? ? ? ? 4D 8B F8 ");
	
	if (smokeEffectAddress == nullptr)
		return false;

	hookInit = MH_CreateHook (
		smokeEffectAddress,
		reinterpret_cast<LPVOID *>(iVisual->m_SmokeEffect.hRenderSmoke), 
		reinterpret_cast<LPVOID *>(&iVisual->m_SmokeEffect.oRenderSmokeParticles));
	
	if(hookInit==MH_OK)
	iHelper->m_Console.printMessage (DEBUG, "\t Smoke Effect HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING SmokeEffect! ");


	uint8_t *flashEffect = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 0F 29 74 24 ? 33 FF 0F");

	hookInit = MH_CreateHook (
		flashEffect,
		reinterpret_cast<LPVOID *>(iVisual->m_FlashEffect.hFlashEffect),
		reinterpret_cast<LPVOID *>(&iVisual->m_FlashEffect.hFlashEffect));

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
		reinterpret_cast<LPVOID *>(iVisual->m_DrawObject.hDrawObject),
		reinterpret_cast<LPVOID *>(&iVisual->m_DrawObject.hDrawObject));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Draw Object HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Draw Object! ");




	int (__fastcall * CreateMaterialFunction)(void *, void *, const char *, void *, unsigned int, unsigned int);

	CreateMaterialFunction = reinterpret_cast<decltype(CreateMaterialFunction)>(iHelper->m_Mem.PatternScanner ("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05"));


	if (!CreateMaterialFunction) {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Create Material! ");
	}


	uint8_t *LightningOverrideAddress = iHelper->m_Mem.PatternScanner ("scenesystem.dll","48 89 54 24 ? 53 41 56 41 57");



	hookInit = MH_CreateHook (
		LightningOverrideAddress,
		reinterpret_cast<LPVOID *>(iVisual->mLightningModulation.hLightningModulation),
		reinterpret_cast<LPVOID *>(&iVisual->mLightningModulation.oLightningModulation));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Lightning HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Lightning! ");



	uint8_t *WorldOverrideAddress = iHelper->m_Mem.PatternScanner ("scenesystem.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9");

	hookInit = MH_CreateHook (
		WorldOverrideAddress,
		reinterpret_cast<LPVOID *>(iHooksManager->m_WorldModulation.hModulateWorldColor),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_WorldModulation.oModulateWorldColor));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t World Modulation HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING World Modulation! ");



	uint8_t *CalculateBones = iHelper->m_Mem.PatternScanner ("client.dll", "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC D0");

	calcBonesFunction calcBones = reinterpret_cast<calcBonesFunction>(CalculateBones);

	if (calcBones) {
		iHelper->m_Console.printMessage (DEBUG, "\t Calc Bones HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Calc Bones! ");
	}




	//uint8_t * validateInput = (uint8_t*)iHelper->m_Mem.GetVMT (g_pInterfaces->pGameInput, 7);

	uint8_t *valiodateInput = iHelper->m_Mem.PatternScanner ("client.dll", "85 D2 0F 85 00");

	hookInit = MH_CreateHook (
		valiodateInput,
		reinterpret_cast<LPVOID *>(iAntiAim->mValidateInput.hValidateInput),
		reinterpret_cast<LPVOID *>(&iAntiAim->mValidateInput.oValidateInput));

	

	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t VALIDATE INPUT HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING VALIDATE INPUT! ");
	}



	uint8_t *overrideCamera = iHelper->m_Mem.PatternScanner ( "client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B FA");

	hookInit = MH_CreateHook (
		overrideCamera,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OverrideViewFunction.hookOverrideView),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OverrideViewFunction.oOverrideViewFunction));


	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t Override Camera HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Override Camera! ");
	}


	MH_EnableHook (MH_ALL_HOOKS);
	return hookInit;
}

void *HooksManager::WorldModulation::hModulateWorldColor(CAggregateSceneObjectWorld *a1, void *a2) {


	if (Globals::worldModulation) {
		int count = a1->count;
		for (int i = 0; i < count; i++) {
			CAggregateSceneObjectDataWorld *pAggregateSceneObjectData = &a1->array[i];

			pAggregateSceneObjectData->r = Globals::worldModulationColor[0];
			pAggregateSceneObjectData->g = Globals::worldModulationColor[1];
			pAggregateSceneObjectData->b = Globals::worldModulationColor[2];
		}

	}

	return oModulateWorldColor (a1, a2);
}

void  HooksManager::OverrideViewClass::hookOverrideView (__int64 a1, CViewSetupTRY * a2) {
	Vec3 test;
	if (Globals::thirdPerson) {

		//a2->fov = globals::fov1;
		//a2->fov2 = globals::fov2;
		//a2->viewmodel = globals::fov3;
		//test = a2->viewAngles;

	}

	return oOverrideViewFunction (a1, a2);

}






bool HooksManager::CreateMove::isPlayerInGame (void)
{
	int gameState = *(int*)(*(uintptr_t*)((uintptr_t)iGameEntitySystem->engine2Dll + (uintptr_t)0x53FCE0) + (uintptr_t)0x228);


	if (gameState == 6)
		return true;

	return false;
}


// a2 = basecmd
void HooksManager::CreateMove::hCreateMove (CCSGOInput *csgoInput, __int64 nSlot, bool bActivate) {
	float dist;

	oCreateMove (csgoInput, nSlot, bActivate);


	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_bExposureControl = true;
	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_flMaxExposure = 1;
	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_flMinExposure = 1;

	if (Globals::AntiAim) {
		//actualcmd = GetUserCmd();
		
		//iHelper->m_Console.printMessage (WARNING, actualcmd);
		//onMove (csgoInput);


	}

	if (iHooksManager->m_CreateMove.isPlayerInGame ( )) {
		iGameEntitySystem->getEnemisByFov ( );

		if (iGameEntitySystem->PlayersVector.size ( ) >= 1 && Globals::CreateMoveHook && GetAsyncKeyState (RI_MOUSE_LEFT_BUTTON_DOWN & 1) && iGameEntitySystem->PlayersVector[0]->Pawn->isInFov) {
			if (iGameEntitySystem->PlayersVector[0]->Pawn->pawnHealth > 0) {

				Vec3 tempEnemy = *(new Vec3 (
					iGameEntitySystem->PlayersVector[0]->Pawn->m_pGameSceneNode->m_vecOrigin.x - iGameEntitySystem->PlayersVector[0]->Pawn->m_vecViewOffset.x,
					iGameEntitySystem->PlayersVector[0]->Pawn->m_pGameSceneNode->m_vecOrigin.y - iGameEntitySystem->PlayersVector[0]->Pawn->m_vecViewOffset.y,
					iGameEntitySystem->PlayersVector[0]->Pawn->m_pGameSceneNode->m_vecOrigin.z - iGameEntitySystem->PlayersVector[0]->Pawn->m_vecViewOffset.z
				));

				Vec3 tempMe = *(new Vec3{
					iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.x - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.x,
					iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.y - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.y,
					iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.z - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.z
					});

				SetViewAngles::hSetViewAngles ((__int64 *)csgoInput, 0, CalculateAngles (tempMe, tempEnemy, Globals::aimbotFov));
			}
		}




		if (Globals::glow) {
			iVisuals->glowPlayers (Globals::glowType, Globals::chamsColor);
		}



	}
}


void HooksManager::CreateMoveTWO::hCreateMoveTWO (CCSGOInput *a1, __int64 nSlot, CUserCmd *a3) {
	oCreateMoveTWO (a1, nSlot, a3);

	if (Globals::AntiAim) {
		iAntiAim->OnMove (a1, a3);
	}

	if (Globals::bhop) {
		iMovement->BunnyHop ( a3, iGameEntitySystem->LocalPlayerPawn);
		iHelper->m_Console.printMessage (WARNING, a3);
	}
}


void HooksManager::SetViewAngles::hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3) {

	return oSetViewAngles (a1, a2, a3);
}

