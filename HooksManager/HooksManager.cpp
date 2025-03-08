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

#include "../visuals/Visuals.h"
#include "../Interfaces/iManager.h"

#include "../Features/Anti-Aim/AntiAim.h"
#include "../Features/Movement/Movement.h"
#include "../Features/Visuals/Visual.h"
#include "../Features/Legitbot/Legitbot.h"
#include "../Features/Misc/Misc.h"

#include "../EntityManager/EntityManager.h"





HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::CreateMoveTWO::CreateMoveFunctionTWO HooksManager::CreateMoveTWO::oCreateMoveTWO = nullptr;

HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;
HooksManager::WorldModulation::ModulateWorldColorFn HooksManager::WorldModulation::oModulateWorldColor = nullptr;
HooksManager::ValidateInput::ValidateInputFunction HooksManager::ValidateInput::oValidateInput = nullptr;
HooksManager::OverrideViewClass::OverrideViewFunction HooksManager::OverrideViewClass::oOverrideViewFunction = nullptr;
HooksManager::FrameStageNotify::FrameStageNotifyFunction HooksManager::FrameStageNotify::oFrameStageNotify = nullptr;
HooksManager::OnAddEntity::OnAddEntityFunction HooksManager::OnAddEntity::oOnAddEntity = nullptr;
HooksManager::OnRemoveEntity::OnRemoveEntityFunction HooksManager::OnRemoveEntity::oOnRemoveEntity = nullptr;


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
		reinterpret_cast<LPVOID *>(&iVisual->m_FlashEffect.oFlashEffect));

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


	uint8_t *createMoveTWO = iHelper->m_Mem.PatternScanner ("client.dll", "48 8B C4 4C 89 40 18 48 89 48 08 55 53 57");

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
		reinterpret_cast<LPVOID *>(&iVisual->m_DrawObject.oDrawObject));

	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage (DEBUG, "\t Draw Object HOOKED! ");
	else
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Draw Object! ");

	iVisual->m_CreateMaterial.CreateMaterialFunction = reinterpret_cast<decltype(iVisual->m_CreateMaterial.CreateMaterialFunction)>(iHelper->m_Mem.PatternScanner ("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05"));


	if (!iVisual->m_CreateMaterial.CreateMaterialFunction) {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING Create Material! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t Create Material HOOKED! ");
	}

	iVisual->m_SetTypeKV3.SetTypeKV3 = reinterpret_cast<decltype(iVisual->m_SetTypeKV3.SetTypeKV3)>(iHelper->m_Mem.PatternScanner ("client.dll", "40 53 48 83 EC 30 48 8B D9 49"));




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


	uint8_t *frameStageNotify = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 5C 24 ? 56 48 83 EC 30 8B 05 ? ? ? ? ");

	hookInit = MH_CreateHook (
		frameStageNotify,
		reinterpret_cast<LPVOID *>(iHooksManager->m_FrameStageNotify.hFrameStageNotify),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_FrameStageNotify.oFrameStageNotify));

	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t FRAME STAGE NOTIFY HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING FRAME STAGE NOTIFY! ");
	}



	uint8_t *onAddEntity = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 B9 ? ? ? ?");

	hookInit = MH_CreateHook (
		onAddEntity,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OnAddEntity.hOnAddEntity),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OnAddEntity.oOnAddEntity));


	uint8_t *onRemoveEntity = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 25 ? ? ? ?");

	hookInit = MH_CreateHook (
		onRemoveEntity,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OnRemoveEntity.hOnRemoveEntity),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OnRemoveEntity.oOnRemoveEntity));

	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t OnRemoveEntity HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING OnRemoveEntity! ");
	}

	uint8_t *overrideCamera = iHelper->m_Mem.PatternScanner ( "client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B FA");

	hookInit = MH_CreateHook (
		overrideCamera,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OverrideViewFunction.hookOverrideView),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OverrideViewFunction.oOverrideViewFunction));


	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t OnAddEntity HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING OnAddEntity! ");
	}



	uint8_t *GetBonePosition = iHelper->m_Mem.PatternScanner ("client.dll", "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1");

	iLegitBot->m_BonePosition.GetBonePositionFunction = reinterpret_cast<decltype(iLegitBot->m_BonePosition.GetBonePositionFunction)>(GetBonePosition);


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

void __fastcall HooksManager::FrameStageNotify::hFrameStageNotify (__int64 a1, int a2) {
	oFrameStageNotify (a1, a2);
}

bool HooksManager::CreateMove::isPlayerInGame (void)
{
	int gameState = *(int*)(*(uintptr_t*)((uintptr_t)iGameEntitySystem->engine2Dll + (uintptr_t)0x53FCE0) + (uintptr_t)0x228);

	if (gameState == 6)
		return true;

	return false;
}


void HooksManager::CreateMove::hCreateMove (CCSGOInput *csgoInput, __int64 nSlot, bool bActivate) {
	float dist;

	oCreateMove (csgoInput, nSlot, bActivate);

	if (isPlayerInGame) {
		Globals::ping = iGameEntitySystem->GetPlayerController ( )->ping;
		if (iGameEntitySystem->PawnMap.size ( ) == 0 && iGameEntitySystem->ObserverMap.size ( ) == 0 && iGameEntitySystem->ControllerMap.size()==0){
			iGameEntitySystem->getGameEntities ( );
		}
	}

	//if (iHooksManager->m_CreateMove.isPlayerInGame ( ) && iGameEntitySystem->LocalPlayerPawn->pawnHealth >0) {
		/*iGameEntitySystem->getEnemisByFov ( );

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
		}*/

	//if (Globals::glow) {
	//	//iVisuals->glowPlayers (Globals::glowType, Globals::chamsColor);
	//	//iGameEntitySystem->getGameEntities ( );

	//	//uint32_t targetHook;
	//	//C_PlayerPawn *localPlayerPawn = *(C_PlayerPawn **)(iGameEntitySystem->clientDll+ 0x188AF20);
	//}
}


void HooksManager::OnAddEntity::hOnAddEntity (__int64 CGameEntitySystem, void *entityPointer, int entityHandle) {
	oOnAddEntity (CGameEntitySystem, entityPointer, entityHandle);

	//std::cout << "CREATE" << iGameEntitySystem->GetSchemaName (entityPointer) << " - " << entityPointer <<  std::endl;
	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "C_CSPlayerPawnBase")==0) {
		iGameEntitySystem->PawnMap.insert (std::make_pair (entityHandle, (C_PlayerPawn *)entityPointer));
		iHelper->m_Console.printMessage (DEBUG, "PAWN ADDED, Pawn size ", iGameEntitySystem->PawnMap.size ( ));
	}
	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "c_cs_observer_for_precache")==0) {
		iGameEntitySystem->ObserverMap.insert (std::make_pair (entityHandle, (C_PlayerPawn *)entityPointer));
		iHelper->m_Console.printMessage (DEBUG, "OBSERVER ADDED, Pawn size ", iGameEntitySystem->ObserverMap.size ( ), " ", entityHandle);

	}
	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "CBasePlayerController")==0) {
		iGameEntitySystem->ControllerMap.insert (std::make_pair (entityHandle, (C_PlayerController *)entityPointer));
		iHelper->m_Console.printMessage (DEBUG, "CONTROLLER ADDED, Pawn size ", iGameEntitySystem->ControllerMap.size ( ), " ", entityHandle);
	}
}
void HooksManager::OnRemoveEntity::hOnRemoveEntity (__int64 CGameEntitySystem, void *entityPointer, int entityHandle) {
	oOnRemoveEntity (CGameEntitySystem, entityPointer, entityHandle);

	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "C_CSPlayerPawnBase")==0) {
		iGameEntitySystem->PawnMap.erase (entityHandle);
		iHelper->m_Console.printMessage (DEBUG, "PAWN REMOVED, Pawn size ", iGameEntitySystem->PawnMap.size ( ));

	}
	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "c_cs_observer_for_precache")==0) {
		iGameEntitySystem->ObserverMap.erase(entityHandle);
		iHelper->m_Console.printMessage (DEBUG, "OBSERVER REMOVED, Pawn size ", iGameEntitySystem->ObserverMap.size ( ));
	}
	if (strcmp (iGameEntitySystem->GetSchemaName (entityPointer).c_str ( ), "CBasePlayerController") == 0) {
		iGameEntitySystem->ControllerMap.erase (entityHandle);
		iHelper->m_Console.printMessage (DEBUG, "CONTROLLER REMOVED, Pawn size ", iGameEntitySystem->ControllerMap.size ( ));
	}

}

void HooksManager::CreateMoveTWO::hCreateMoveTWO (CCSGOInput *a1, __int64 nSlot, CUserCmd *a3) {
	oCreateMoveTWO (a1, nSlot, a3);


	if (!a3)
		return;

	if (Globals::AntiAim) {
		iAntiAim->OnMove (a1, a3);
	}
	
	if (Globals::bhop) {
		iMovement->BunnyHop ( a3, iGameEntitySystem->GetPlayerPawn());
	}

	if (Globals::ShowSpectators) {
		iMisc->m_Spectators.ShowSpectatorList (iGameEntitySystem->PawnMap, iGameEntitySystem->ControllerMap, iGameEntitySystem->ObserverMap, iGameEntitySystem->GetPlayerPawn(), iGameEntitySystem->GetPlayerController ());
	}

	return;
}


void HooksManager::SetViewAngles::hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3) {

	return oSetViewAngles (a1, a2, a3);
}

