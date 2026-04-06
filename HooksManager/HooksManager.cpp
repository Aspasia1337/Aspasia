#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "HooksManager.h"
#include "../Helper/helper.h"
#include "../Includes/kiero/minhook/include/MinHook.h"
#include "../globals.h"
#include "../Math/vector.h"
#include "../Interfaces/iManager.h"
#include "../Features/Anti-Aim/AntiAim.h"
#include "../Features/Movement/Movement.h"
#include "../Features/Visuals/Visual.h"
#include "../Features/Legitbot/Legitbot.h"
#include "../Features/Misc/Misc.h"
#include "../EntityManager/EntityManager.h"
#include "../Interfaces/iManager.h"

HooksManager::CreateMove::CreateMoveFunction HooksManager::CreateMove::oCreateMove = nullptr;
HooksManager::CreateMoveTWO::CreateMoveFunctionTWO HooksManager::CreateMoveTWO::oCreateMoveTWO = nullptr;
HooksManager::SetViewAngles::SetViewAnglesFunction HooksManager::SetViewAngles::oSetViewAngles = nullptr;
HooksManager::ValidateInput::ValidateInputFunction HooksManager::ValidateInput::oValidateInput = nullptr;
HooksManager::OverrideViewClass::OverrideViewFunction HooksManager::OverrideViewClass::oCameraServices = nullptr;
HooksManager::FrameStageNotify::FrameStageNotifyFunction HooksManager::FrameStageNotify::oFrameStageNotify = nullptr;
HooksManager::OnAddEntity::OnAddEntityFunction HooksManager::OnAddEntity::oOnAddEntity = nullptr;
HooksManager::OnRemoveEntity::OnRemoveEntityFunction HooksManager::OnRemoveEntity::oOnRemoveEntity = nullptr;
HooksManager::IsRelativeMouseMode::IsRelativeMouseModeFunction HooksManager::IsRelativeMouseMode::oIsRelativeMouseMode = nullptr;
HooksManager::IsRelativeMouseMode::MouseInputFunction HooksManager::IsRelativeMouseMode::oMouseInput = nullptr;
AntiAim::ValidateInput::ValidateInputFunction AntiAim::ValidateInput::oValidateInput = nullptr;
Visual::LightningModulation::LightningModulationFunction Visual::LightningModulation::oLightningModulation = nullptr;
Visual::SmokeRender::RenderSmokeParticlesFunction Visual::SmokeRender::oRenderSmokeParticles = nullptr;
Visual::FlashEffect::FlashEffectFunction Visual::FlashEffect::oFlashEffect = nullptr;
Visual::DrawObjectClass::DrawObjectFunction Visual::DrawObjectClass::oDrawObject = nullptr;
Visual::WorldModulation::ModulateWorldColorFn Visual::WorldModulation::oModulateWorldColor = nullptr;
Misc::ScopeRemoval::RemoveScopeFN Misc::ScopeRemoval::oRemoveScopeFn = nullptr;
CKeyValues3 *(__fastcall *Visual::Chams::SetTypeKV3)(CKeyValues3 *, int, unsigned int) = nullptr;
bool(__fastcall *Visual::Chams::LoadKeyValues)(CKeyValues3 *, void *, CUtilsBuff *, KV3ID_t *, void *, void *, void *, void *, const char *) = nullptr;
int64_t(__fastcall *Visual::Chams::CreateMaterialFunction)(void *, void *, const char *, void *, unsigned int, unsigned int) = nullptr;
CUtilsBuff *(__fastcall *Visual::Chams::BuffInit)(CUtilsBuff *, int a1, int nSize, int a3) = nullptr;
void(__fastcall *Visual::Chams::BuffPutString)(CUtilsBuff *, const char *) = nullptr;
void(__fastcall *CInterfaceManager::setPVS)(CPVS *, bool);
int HooksManager::mutex = 0;
Vec3 AntiAim::PlayerAngles;
HooksManager::calcBonesFunction calcBones = nullptr;
Vec3 antiAimAngles;

bool HooksManager::initHook()
{
	iHelper->m_Console.printMessage(INFO, "[HookManager] Init");
	MH_STATUS hookInit;
	MH_Initialize();
	uint8_t *smokeEffectAddress = iHelper->m_Mem.PatternScanner("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B 9C 24 ? ? ? ? 4D 8B F8 ");
	if (smokeEffectAddress == nullptr)
		return false;
	hookInit = MH_CreateHook(
		smokeEffectAddress,
		reinterpret_cast<LPVOID *>(iVisual->m_SmokeEffect.hRenderSmoke),
		reinterpret_cast<LPVOID *>(&iVisual->m_SmokeEffect.oRenderSmokeParticles));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] SmokeEffect hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook SmokeEffect (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *flashEffect = iHelper->m_Mem.PatternScanner("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 0F 29 74 24 ? 33 FF 0F");
	hookInit = MH_CreateHook(
		flashEffect,
		reinterpret_cast<LPVOID *>(iVisual->m_FlashEffect.hFlashEffect),
		reinterpret_cast<LPVOID *>(&iVisual->m_FlashEffect.oFlashEffect));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] FlashEffect hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook FlashEffect (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *createMove = iHelper->m_Mem.PatternScanner("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18");
	hookInit = MH_CreateHook(
		createMove,
		reinterpret_cast<LPVOID *>(iHooksManager->m_CreateMove.hCreateMove),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_CreateMove.oCreateMove));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] CreateMove hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook CreateMove (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *createMoveTWO = iHelper->m_Mem.PatternScanner("client.dll", "48 8B C4 4C 89 40 18 48 89 48 08 55 53 57");
	hookInit = MH_CreateHook(
		createMoveTWO,
		reinterpret_cast<LPVOID *>(iHooksManager->m_CreateMoveTWO.hCreateMoveTWO),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_CreateMoveTWO.oCreateMoveTWO));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] CreateMoveTWO hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook CreateMoveTWO (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *setViewAngles = iHelper->m_Mem.PatternScanner("client.dll", "85 D2 75 3F 48");
	hookInit = MH_CreateHook(
		setViewAngles,
		reinterpret_cast<LPVOID *>(iHooksManager->m_SetViewAngles.hSetViewAngles),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_SetViewAngles.oSetViewAngles));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] SetViewAngles hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook SetViewAngles (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *drawObjectaddrr = iHelper->m_Mem.PatternScanner("scenesystem.dll", "48 83 EC 48 48 8B 84 24 ? ? ? ? 48 8D 0D ? ? ? ?");
	hookInit = MH_CreateHook(
		drawObjectaddrr,
		reinterpret_cast<LPVOID *>(iVisual->m_DrawObject.hDrawObject),
		reinterpret_cast<LPVOID *>(&iVisual->m_DrawObject.oDrawObject));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] DrawObject hooked (scenesystem.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook DrawObject (scenesystem.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *LightningOverrideAddress = iHelper->m_Mem.PatternScanner("scenesystem.dll", "48 89 54 24 ? 53 41 56 41 57");
	hookInit = MH_CreateHook(
		LightningOverrideAddress,
		reinterpret_cast<LPVOID *>(iVisual->mLightningModulation.hLightningModulation),
		reinterpret_cast<LPVOID *>(&iVisual->mLightningModulation.oLightningModulation));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] LightningModulation hooked (scenesystem.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook LightningModulation (scenesystem.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *WorldOverrideAddress = iHelper->m_Mem.PatternScanner("scenesystem.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9");
	hookInit = MH_CreateHook(
		WorldOverrideAddress,
		reinterpret_cast<LPVOID *>(iVisual->m_WorldModulation.hModulateWorldColor),
		reinterpret_cast<LPVOID *>(&iVisual->m_WorldModulation.oModulateWorldColor));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] WorldModulation hooked (scenesystem.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook WorldModulation (scenesystem.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *CalculateBones = iHelper->m_Mem.PatternScanner("client.dll", "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC D0");
	calcBonesFunction calcBones = reinterpret_cast<calcBonesFunction>(CalculateBones);
#ifdef DEBUG_MODE
	if (calcBones)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] CalcBones resolved (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve CalcBones (client.dll)");
#endif
	uint8_t *isRelativeMouse = (uint8_t *)iHelper->m_Mem.GetVMT(g_pInterfaces->pGameInput, 77);
	hookInit = MH_CreateHook(
		isRelativeMouse,
		reinterpret_cast<LPVOID *>(iHooksManager->m_IsRelativeMouseMode.hIsRelativeMouseFunction),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_IsRelativeMouseMode.oIsRelativeMouseMode));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] IsRelativeMouseMode hooked (VMT idx 77)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook IsRelativeMouseMode (VMT idx 77): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *mouseInput = (uint8_t *)iHelper->m_Mem.GetVMT(g_pInterfaces->pGameInput, 17);
	hookInit = MH_CreateHook(
		mouseInput,
		reinterpret_cast<LPVOID *>(iHooksManager->m_IsRelativeMouseMode.hMouseInput),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_IsRelativeMouseMode.oMouseInput));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] MouseInput hooked (VMT idx 17)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook MouseInput (VMT idx 17): MH_CreateHook => %d", hookInit);
#endif
	// uint8_t * validateInput = (uint8_t*)iHelper->m_Mem.GetVMT (g_pInterfaces->pGameInput, 7);
	uint8_t *valiodateInput = iHelper->m_Mem.PatternScanner("client.dll", "85 D2 0F 85 00");
	hookInit = MH_CreateHook(
		valiodateInput,
		reinterpret_cast<LPVOID *>(iAntiAim->mValidateInput.hValidateInput),
		reinterpret_cast<LPVOID *>(&iAntiAim->mValidateInput.oValidateInput));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] ValidateInput hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook ValidateInput (client.dll): MH_CreateHook => %d", hookInit);
#endif
	iVisual->m_UpdateSkybox.UpdateSkyboxFunction = reinterpret_cast<decltype(iVisual->m_UpdateSkybox.UpdateSkyboxFunction)>(iHelper->m_Mem.PatternScanner("client.dll", "48 89 5C 24 08 57 48 83 EC 30 48 8B F9 E8 ?? ?? ?? ?? 48 8B 47"));
#ifdef DEBUG_MODE
	if (iVisual->m_UpdateSkybox.UpdateSkyboxFunction)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] UpdateSkybox resolved (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve UpdateSkybox (client.dll)");
#endif
	uint8_t *frameStageNotify = iHelper->m_Mem.PatternScanner("client.dll", "48 89 5C 24 ? 56 48 83 EC 30 8B 05 ? ? ? ? ");
	hookInit = MH_CreateHook(
		frameStageNotify,
		reinterpret_cast<LPVOID *>(iHooksManager->m_FrameStageNotify.hFrameStageNotify),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_FrameStageNotify.oFrameStageNotify));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] FrameStageNotify hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook FrameStageNotify (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *onAddEntity = iHelper->m_Mem.PatternScanner("client.dll", "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 B9 ? ? ? ?");
	hookInit = MH_CreateHook(
		onAddEntity,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OnAddEntity.hOnAddEntity),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OnAddEntity.oOnAddEntity));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] OnAddEntity hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook OnAddEntity (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *onRemoveEntity = iHelper->m_Mem.PatternScanner("client.dll", "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 25 ? ? ? ?");
	hookInit = MH_CreateHook(
		onRemoveEntity,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OnRemoveEntity.hOnRemoveEntity),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OnRemoveEntity.oOnRemoveEntity));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] OnRemoveEntity hooked (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook OnRemoveEntity (client.dll): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *CameraServices = iHelper->m_Mem.PatternScanner("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 48 8B FA");
	hookInit = MH_CreateHook(
		CameraServices,
		reinterpret_cast<LPVOID *>(iHooksManager->m_OverrideViewFunction.hCameraServices),
		reinterpret_cast<LPVOID *>(&iHooksManager->m_OverrideViewFunction.oCameraServices));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] OverrideView hooked (CameraServices)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook OverrideView (CameraServices): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *GetBonePosition = iHelper->m_Mem.PatternScanner("client.dll", "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1");
	iLegitBot->m_BonePosition.GetBonePositionFunction = reinterpret_cast<decltype(iLegitBot->m_BonePosition.GetBonePositionFunction)>(GetBonePosition);
	uint8_t *SetKV3 = iHelper->m_Mem.PatternScanner("client.dll", "40 53 48 83 EC 30 48 8B D9 49");
	iVisual->m_Chams.SetTypeKV3 = reinterpret_cast<decltype(iVisual->m_Chams.SetTypeKV3)>(SetKV3);
#ifdef DEBUG_MODE
	if (iVisual->m_Chams.SetTypeKV3)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] SetTypeKV3 resolved (client.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve SetTypeKV3 (client.dll)");
#endif
	uint8_t *LoadKeyValues = iHelper->m_Mem.GetAbsoluteAddress(iHelper->m_Mem.PatternScanner("tier0.dll", "E8 ? ? ? ? EB 36 8B 43 10"), 0x1, 0x0);
	iVisual->m_Chams.LoadKeyValues = reinterpret_cast<decltype(iVisual->m_Chams.LoadKeyValues)>(LoadKeyValues);
#ifdef DEBUG_MODE
	if (iVisual->m_Chams.LoadKeyValues)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] LoadKeyValues resolved (tier0.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve LoadKeyValues (tier0.dll)");
#endif
	uint8_t *CreateMaterialFunction = iHelper->m_Mem.PatternScanner("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05");
	iVisual->m_Chams.CreateMaterialFunction = reinterpret_cast<decltype(iVisual->m_Chams.CreateMaterialFunction)>(CreateMaterialFunction);
#ifdef DEBUG_MODE
	if (iVisual->m_Chams.CreateMaterialFunction)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] CreateMaterialFunction resolved (materialsystem2.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve CreateMaterialFunction (materialsystem2.dll)");
#endif
	uint8_t *BufInit = iHelper->m_Mem.PatternScanner("tier0.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 20 33 DB 44 89 41 08");
	iVisual->m_Chams.BuffInit = reinterpret_cast<decltype(iVisual->m_Chams.BuffInit)>(BufInit);
#ifdef DEBUG_MODE
	if (iVisual->m_Chams.BuffInit)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] BuffInit resolved (tier0.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve BuffInit (tier0.dll)");
#endif
	uint8_t *BuffCopy = iHelper->m_Mem.PatternScanner("tier0.dll", "48 89 5C 24 ? 57 48 83 EC 20 0F B6 41 19");
	iVisual->m_Chams.BuffPutString = reinterpret_cast<decltype(iVisual->m_Chams.BuffPutString)>(BuffCopy);
#ifdef DEBUG_MODE
	if (iVisual->m_Chams.BuffPutString)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] BuffPutString resolved (tier0.dll)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to resolve BuffPutString (tier0.dll)");
#endif
	iVisual->m_Chams.InitChams();
	iHelper->m_Console.printMessage(WARNING, "[HookManager] Materials initialized");
	uint8_t *ViewModelFov = iHelper->m_Mem.PatternScanner("client.dll", "4C 8B DC 53 56 57 48 83 EC");
	hookInit = MH_CreateHook(
		ViewModelFov,
		reinterpret_cast<LPVOID *>(iMisc->m_ScopeRemoval.hRemoveScope),
		reinterpret_cast<LPVOID *>(&iMisc->m_ScopeRemoval.oRemoveScopeFn));
#ifdef DEBUG_MODE
	if (hookInit == MH_OK)
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] RemoveScope hooked (SetFovFn)");
	else
		iHelper->m_Console.printMessage(WARNING, "[HookManager] Failed to hook RemoveScope (SetFovFn): MH_CreateHook => %d", hookInit);
#endif
	uint8_t *PVS = (iHelper->m_Mem.ResolveRelativeAddress(iHelper->m_Mem.PatternScanner("engine2.dll", "48 8D 0D ? ? ? ? 33 ? FF 50"), 0x3, 0x7));
	g_pInterfaces->pPVS = reinterpret_cast<decltype(g_pInterfaces->pPVS)>(PVS);
	uint8_t *setPVS = iHelper->m_Mem.PatternScanner("engine2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 88 51 2C");
	g_pInterfaces->setPVS = reinterpret_cast<decltype(g_pInterfaces->setPVS)>(setPVS);
	MH_EnableHook(MH_ALL_HOOKS);
	return hookInit;
}

void HooksManager::OverrideViewClass::hCameraServices(__int64 a1, CViewSetupTRY *a2)
{
	Vec3 test;
	float exposure;

	return oCameraServices(a1, a2);
}

void __fastcall HooksManager::FrameStageNotify::hFrameStageNotify(__int64 a1, int a2)
{
	oFrameStageNotify(a1, a2);
}

bool HooksManager::CreateMove::isPlayerInGame(void)
{
	int gameState = *(int *)(*(uintptr_t *)((uintptr_t)iGameEntitySystem->engine2Dll + (uintptr_t)0x53FCE0) + (uintptr_t)0x228);

	if (gameState == 6)
		return true;

	return false;
}

void HooksManager::CreateMove::hCreateMove(CCSGOInput *csgoInput, __int64 nSlot, bool bActivate)
{
	float dist;

	oCreateMove(csgoInput, nSlot, bActivate);

	if (isPlayerInGame)
	{
		Globals::ping = iGameEntitySystem->GetPlayerController()->ping;
		
		// In some cases, such as rejoining an already active game, OnAddGameEntity might not be triggered.
		// To avoid missing entities, we fetch all game entities manually and populate our hash maps.
		// After this initial population, we rely on OnAddEntity and OnRemoveEntity for updates.
		
		if (iGameEntitySystem->PawnMap.size() <= 1 && iGameEntitySystem->ObserverMap.size() <= 1 && iGameEntitySystem->ControllerMap.size() <= 1)
		{
			iGameEntitySystem->getGameEntities();
		}

		if (Globals::CreateMoveHook && iGameEntitySystem->PawnMap.size() >= 1)
		{
			// C_PlayerPawn *EnemyTemp = nullptr;
			//	for(auto &anyenemy: iGameEntitySystem->PawnMap){
			//		if (anyenemy.second != iGameEntitySystem->GetPlayerPawn ( ) && !anyenemy.second->pawnIsAlive && anyenemy.second->pawnHealth > 0)
			//		{
			//			EnemyTemp = anyenemy.second;
			//			break;
			//		}
			//	}

			C_PlayerPawn *EnemyTemp = nullptr;
			double MinDistance = 99999999;

			Vec3 PlayerPos = iGameEntitySystem->GetPlayerPawn()->vOldOrigin;

			// Select the closest enemy
			for (auto &anyenemy : iGameEntitySystem->PawnMap)
			{
				C_PlayerPawn *enemy = anyenemy.second;

				if (enemy != iGameEntitySystem->GetPlayerPawn() && !enemy->pawnIsAlive && enemy->pawnHealth > 0 && !enemy->m_pGameSceneNode->m_bDormant)
				{
					double distance = CalculateDistance(PlayerPos, enemy->vOldOrigin);

					if (distance < MinDistance)
					{
						MinDistance = distance;
						EnemyTemp = enemy;
					}
				}
			}

			if (!EnemyTemp || EnemyTemp->pawnIsAlive && EnemyTemp->pawnHealth <= 0 && !EnemyTemp->pawnIsAlive)
				return;

			Vec3 tempMe = Vec3(
				iGameEntitySystem->GetPlayerPawn()->m_pGameSceneNode->m_vecOrigin.x + iGameEntitySystem->GetPlayerPawn()->m_vecViewOffset.x,
				iGameEntitySystem->GetPlayerPawn()->m_pGameSceneNode->m_vecOrigin.y + iGameEntitySystem->GetPlayerPawn()->m_vecViewOffset.y,
				iGameEntitySystem->GetPlayerPawn()->m_pGameSceneNode->m_vecOrigin.z + iGameEntitySystem->GetPlayerPawn()->m_vecViewOffset.z);

			Vec3 pos = iLegitBot->m_BonePosition.GetBonePosFromIndex(Globals::boneMap.at(Globals::BoneNames[Globals::BoneSelected]), EnemyTemp, iGameEntitySystem->GetPlayerPawn());

			Vec3 tempEnemy = Vec3(
				pos.x,
				pos.y,
				pos.z);

			SetViewAngles::hSetViewAngles((__int64 *)csgoInput, 0, CalculateAngles(tempMe, tempEnemy, Globals::aimbotFov));
		}

		if (Globals::PlayersChams && Globals::PVS && HooksManager::mutex == 0)
		{
			g_pInterfaces->setPVS(g_pInterfaces->pPVS, false);
			HooksManager::mutex = 1;
		}
		else if (Globals::PlayersChams && !Globals::PVS && HooksManager::mutex == 1)
		{
			g_pInterfaces->setPVS(g_pInterfaces->pPVS, true);
			HooksManager::mutex = 0;
		}
	}
}

void *HooksManager::IsRelativeMouseMode::hIsRelativeMouseFunction(void *pThisptr, bool bActive)
{
	Globals::MouseEvent = pThisptr;
	Globals::LastMode = bActive;
	if (Globals::showMenu)
	{
		return oIsRelativeMouseMode(pThisptr, false);
	}
	return oIsRelativeMouseMode(pThisptr, bActive);
}

void *savedPtr;
void HooksManager::IsRelativeMouseMode::hMouseInput(void *pThisptr)
{
	if (Globals::showMenu)
	{
		return;
	}
	oMouseInput(pThisptr);
	return;
}

void HooksManager::OnAddEntity::hOnAddEntity(__int64 CGameEntitySystem, void *entityPointer, int entityHandle)
{
	oOnAddEntity(CGameEntitySystem, entityPointer, entityHandle);

	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "C_CSPlayerPawnBase") == 0)
	{
		iGameEntitySystem->PawnMap.insert(std::make_pair(entityHandle, (C_PlayerPawn *)entityPointer));
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Pawn added: handle=", entityHandle, ", total=", iGameEntitySystem->PawnMap.size());
#endif
	}
	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "c_cs_observer_for_precache") == 0)
	{
		iGameEntitySystem->ObserverMap.insert(std::make_pair(entityHandle, (C_PlayerPawn *)entityPointer));
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Observer added: handle=", entityHandle, ", total=", iGameEntitySystem->ObserverMap.size());
#endif
	}
	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "CBasePlayerController") == 0)
	{
		iGameEntitySystem->ControllerMap.insert(std::make_pair(entityHandle, (C_PlayerController *)entityPointer));
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Controller added: handle=", entityHandle, ", total=", iGameEntitySystem->ControllerMap.size());
#endif
	}
}
void HooksManager::OnRemoveEntity::hOnRemoveEntity(__int64 CGameEntitySystem, void *entityPointer, int entityHandle)
{
	oOnRemoveEntity(CGameEntitySystem, entityPointer, entityHandle);

	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "C_CSPlayerPawnBase") == 0)
	{
		iGameEntitySystem->PawnMap.erase(entityHandle);
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Pawn removed: handle=", entityHandle, ", total=", iGameEntitySystem->PawnMap.size());
#endif
	}
	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "c_cs_observer_for_precache") == 0)
	{
		iGameEntitySystem->ObserverMap.erase(entityHandle);
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Observer removed: handle=", entityHandle, ", total=", iGameEntitySystem->ObserverMap.size());
#endif
	}
	if (strcmp(iGameEntitySystem->GetSchemaName(entityPointer).c_str(), "CBasePlayerController") == 0)
	{
		iGameEntitySystem->ControllerMap.erase(entityHandle);
#ifdef DEBUG_MODE
		iHelper->m_Console.printMessage(DEBUG, "[HookManager] Controller removed: handle=", entityHandle, ", total=", iGameEntitySystem->ControllerMap.size());
#endif
	}
}

void HooksManager::CreateMoveTWO::hCreateMoveTWO(CCSGOInput *a1, __int64 nSlot, CUserCmd *a3)
{
	oCreateMoveTWO(a1, nSlot, a3);

	if (!a3)
		return;
	if (Globals::AntiAim || Globals::ManualAA)
	{
		iAntiAim->OnMove(a1, a3);
	}
	if (Globals::bhop)
	{
		iMovement->BunnyHop(a3, iGameEntitySystem->GetPlayerPawn());
	}
	if (Globals::ShowSpectators)
	{
		iMisc->m_Spectators.ShowSpectatorList(iGameEntitySystem->PawnMap, iGameEntitySystem->ControllerMap, iGameEntitySystem->ObserverMap, iGameEntitySystem->GetPlayerPawn(), iGameEntitySystem->GetPlayerController());
	}
	if (Globals::ChangeSkyColor)
	{
		iVisual->m_UpdateSkybox.ChangeSkybox();
	}
}

void HooksManager::SetViewAngles::hSetViewAngles(__int64 *a1, __int64 a2, Vec3 a3)
{
	return oSetViewAngles(a1, a2, a3);
}
