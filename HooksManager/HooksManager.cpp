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

HooksManager::SmokeEffect::RenderSmokeParticlesFunction HooksManager::SmokeEffect::oRenderSmokeParticles = nullptr;
HooksManager::FlashEffect::FlashEffectFunction HooksManager::FlashEffect::oFlashEffect = nullptr;
HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::CreateMoveTWO::CreateMoveFunctionTWO HooksManager::CreateMoveTWO::oCreateMoveTWO = nullptr;

HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;
HooksManager::DrawObjectClass::DrawObjectFunction HooksManager::DrawObjectClass::oDrawObject = nullptr;
HooksManager::LightningModulation::LightningModulationFunction HooksManager::LightningModulation::oLightningModulation = nullptr;
HooksManager::WorldModulation::oModulateWorldColorFn HooksManager::WorldModulation::oModulateWorldColor = nullptr;
HooksManager::ValidateInput::ValidateInputFunction HooksManager::ValidateInput::oValidateInput = nullptr;
HooksManager::OverrideViewClass::OverrideViewFunction HooksManager::OverrideViewClass::oOverrideViewFunction = nullptr;


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


	uint8_t *LightningOverrideAddress = iHelper->m_Mem.PatternScanner ("scenesystem.dll","48 89 54 24 ? 53 41 56 41 57");



	hookInit = MH_CreateHook (
		LightningOverrideAddress,
		reinterpret_cast<LPVOID *>(iHooksManager->m_LightningModulation.hLightningModulation),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_LightningModulation.oLightningModulation));

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
		reinterpret_cast<LPVOID *>(iHooksManager->m_ValidateInput.hValidateInput),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_ValidateInput.oValidateInput));

	

	if (hookInit == MH_OK) {
		iHelper->m_Console.printMessage (DEBUG, "\t VALIDATE INPUT HOOKED! ");
	}
	else {
		iHelper->m_Console.printMessage (WARNING, "\t ERROR HOOCKING VALIDATE INPUT! ");
	}



	uint8_t *overrideCamera = iHelper->m_Mem.PatternScanner ( "client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8");

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


	if (globals::worldModulation) {
		int count = a1->count;
		for (int i = 0; i < count; i++) {
			CAggregateSceneObjectDataWorld *pAggregateSceneObjectData = &a1->array[i];

			pAggregateSceneObjectData->r = globals::worldModulationColor[0];
			pAggregateSceneObjectData->g = globals::worldModulationColor[1];
			pAggregateSceneObjectData->b = globals::worldModulationColor[2];
		}

	}

	return oModulateWorldColor (a1, a2);
}

void  HooksManager::OverrideViewClass::hookOverrideView (__int64 a1, CViewSetupTRY * a2) {
	Vec3 test;
	if (globals::thirdPerson) {

		//a2->fov = globals::fov1;
		//a2->fov2 = globals::fov2;
		//a2->viewmodel = globals::fov3;
		//test = a2->viewAngles;
		test = *(new Vec3 (
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.x - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.x,
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.y - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.y,
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.z - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.z
		));
	}

	/*
		if (globals::antiAim) {

		test = *(new Vec3 (
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.x - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.x,
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.y - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.y,
			iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.z - iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.z
		));

		a2->position = test;
		a2->viewAngles = iGameEntitySystem->LocalPlayerPawn->v_angle;
	}
	*/


	return oOverrideViewFunction (a1, a2);

}

void* HooksManager::LightningModulation::hLightningModulation (__int64 a1, CAggregateSceneObject *a2, __int64 a3) {
	
	if (globals::lightModulation) {
		a2->lightType = globals::lightType;
		a2->redColor = globals::lightModulationColor[0] / globals::lightIntensity;
		a2->greenColor = globals::lightModulationColor[1] / globals::lightIntensity;
		a2->blueColor = globals::lightModulationColor[2] / globals::lightIntensity;




	}
	return oLightningModulation (a1, a2, a3);
}



void HooksManager::SmokeEffect::hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6) {
	
	if (globals::RenderSmokeHook) {
		return;
	}

	return (oRenderSmokeParticles (a1,  a2,  a3,  a4,  a5,  a6));

}

/*
   //Im hooking this
	OnDrawObject(void * pAnimatableSceneObjectDesc, void * pDx11, CMeshData * arrMeshDraw, int nDataCount, void * pSceneView, void * pSceneLayer, void * pUnk, void * pUnk2)

	//Drawing the chams like this

	if (bIgnoreZ) {
	  arrMeshDraw -> pMaterial = pMaterialInvisible;
	  arrMeshDraw -> colValue = colVisualChamsIgnoreZ;
	  oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
	}

	arrMeshDraw -> pMaterial = pMaterial;
	arrMeshDraw -> colValue = colVisualChams;
	oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

*/
void HooksManager::DrawObjectClass::hDrawObject (void *a1, void *a2, void *a3, int a4, void *a5, void *a6, void *a7, void *a8) {


	return oDrawObject (a1, a2, a3, a4, a5,a6, a7, a8);
}

void HooksManager::FlashEffect::hFlashEffect (__int64 a1, __int64 a2, float* a3) {



	if (globals::RenderFlashHook) {
		//iHelper->m_Console.printMessage(WARNING, calcBones (iGameEntitySystem->LocalPlayerPawn->m_pGameSceneNode, 1));

		return;
	}

	return (oFlashEffect (a1, a2, a3));
}




CUserCmd *actualcmd;

void HooksManager::ValidateInput::hValidateInput (CCSGOInput *csgoInput, __int64 a2){

	Vec3 qangles;

	if (globals::antiAim) {

		// Backup Angles
		//qangles = csgoInput->angles;
		// Set Angles to Anti Aim 
		//iHooksManager->m_SetViewAngles.hSetViewAngles ((__int64 *)csgoInput, 0, antiAimAngles);
		// Run Original
		qangles = csgoInput->angles;

		csgoInput->angles = antiAimAngles;

		oValidateInput (csgoInput, a2);

		csgoInput->angles = qangles;

		// Restore Originals
		
		//iHooksManager->m_SetViewAngles.hSetViewAngles ((__int64 *)csgoInput, 0, qangles);
	}else {
		oValidateInput (csgoInput, a2);
	}
}

float spinCounter = 0.f;

void onMove (CCSGOInput *csgoInput) {
	//Store the viewangle for looking arround
	
	antiAimAngles = actualcmd->cbaseusercmd->msgqangle->angles;

	spinCounter += 10;

	//set the view angle backwards
	actualcmd->cbaseusercmd->msgqangle->angles = *new Vec3{90,spinCounter,0};


	//csgoInput->angles = *new Vec3(90,180,0);
 
}

bool HooksManager::CreateMove::isPlayerInGame (void)
{
	int gameState = *(int*)(*(uintptr_t*)((uintptr_t)iGameEntitySystem->engine2Dll + (uintptr_t)0x53FCE0) + (uintptr_t)0x228);


	if (gameState == 6)
		return true;

	return false;
}
/////////////////////////



template <typename T = std::uint8_t>
[[nodiscard]] T *GetAbsoluteAddress (T *pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
{
	pRelativeAddress += nPreOffset;
	pRelativeAddress += sizeof (std::int32_t) + *reinterpret_cast<std::int32_t *>(pRelativeAddress);
	pRelativeAddress += nPostOffset;
	return pRelativeAddress;
}




/////////////////////////

uint8_t * ResolveRelativeAddress (std::uint8_t *nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
{
	std::uint32_t nRVA = *reinterpret_cast<std::uint32_t *>(nAddressBytes + nRVAOffset);
	std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

	return reinterpret_cast<std::uint8_t *>(nRVA + nRIP);
}

CUserCmd *get_user_cmd ( )
{
	static auto get_command_index = reinterpret_cast<void *(__fastcall *)(void *, int *)>(GetAbsoluteAddress (iHelper->m_Mem.PatternScanner ("client.dll", "E8 ? ? ? ? 8B 8D ? ? ? ? 8D 51"), 1, 0));
	if (!get_command_index)
		return nullptr;

	int index = 0;
	get_command_index (iGameEntitySystem->LocalPlayerController, &index);
	int command_index = index - 1;

	if (command_index == -1)
		command_index = 0xFFFFFFFFLL;

	static auto get_user_cmd_base = reinterpret_cast<void *(__fastcall *)(void *, int)>(GetAbsoluteAddress (iHelper->m_Mem.PatternScanner ("client.dll", "E8 ? ? ? ? 48 8B CF 4C 8B E8 44 8B B8"), 1, 0));
	if (!get_user_cmd_base)
		return nullptr;

	static void *cmd_base_address = *reinterpret_cast<void **>(ResolveRelativeAddress (iHelper->m_Mem.PatternScanner ("client.dll", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8"), 0x3, 0x7));
	if (!cmd_base_address)
		return nullptr;

	auto user_cmd_base = get_user_cmd_base (cmd_base_address, command_index);
	if (!user_cmd_base)
		return nullptr;

	DWORD sequence_number = *reinterpret_cast<DWORD *>((uintptr_t)user_cmd_base + 0x5C00);

	static auto get_user_cmd = reinterpret_cast<CUserCmd * (__fastcall *)(void *, DWORD)>(GetAbsoluteAddress (iHelper->m_Mem.PatternScanner ("client.dll", "E8 ? ? ? ? 48 8B 0D ? ? ? ? 45 33 E4 48 89 44 24"), 1, 0));
	if (!get_user_cmd)
		return nullptr;

	auto user_cmd = get_user_cmd (iGameEntitySystem->LocalPlayerController, sequence_number);
	return user_cmd;
}


/////////////////////////

// a2 = basecmd
void HooksManager::CreateMove::hCreateMove (CCSGOInput *csgoInput, __int64 nSlot, bool bActivate) {
	float dist;

	oCreateMove (csgoInput, nSlot, bActivate);


	iGameEntitySystem->getEnemisByFov ( );



	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_bExposureControl = true;
	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_flMaxExposure = 1;
	//iGameEntitySystem->LocalPlayerPawn->m_pCameraServices->m_hActivePostProcessingVolume.m_flMinExposure = 1;

	if (globals::antiAim) {
		//actualcmd = GetUserCmd();
		
		//iHelper->m_Console.printMessage (WARNING, actualcmd);
		onMove (csgoInput);


	}



	if (iHooksManager->m_CreateMove.isPlayerInGame ( )) {
		iGameEntitySystem->getEnemisByFov ( );

		if (iGameEntitySystem->PlayersVector.size ( ) >= 1 && globals::CreateMoveHook && GetAsyncKeyState (RI_MOUSE_LEFT_BUTTON_DOWN & 1) && iGameEntitySystem->PlayersVector[0]->Pawn->isInFov) {
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

				SetViewAngles::hSetViewAngles ((__int64 *)csgoInput, 0, CalculateAngles (tempMe, tempEnemy, globals::aimbotFov));
			}
		}




		if (globals::glow) {
			iVisuals->glowPlayers (globals::glowType, globals::chamsColor);
		}



	}
}




// a2 = basecmd
void HooksManager::CreateMoveTWO::hCreateMoveTWO (CCSGOInput *a1, __int64 nSlot, CUserCmd *a3) {
	oCreateMoveTWO (a1, nSlot, a3);
	actualcmd = a3;

	if (globals::bhop) {
		if (iGameEntitySystem->LocalPlayerPawn->m_fFlags & 257 && a3->buttons & (1 << 1)) {
			a3->buttons &= ~(1 << 1);
		}
	}
		
}
void HooksManager::SetViewAngles::hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3) {



	return oSetViewAngles (a1, a2, a3);

}

