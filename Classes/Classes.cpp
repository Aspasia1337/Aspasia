#include "Classes.h"

C_PlayerPawn *C_PlayerPawn::GetPawn(std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap)
{
	for (auto &pawns : PawnMap)
	{
		if (this->m_hOriginalController == pawns.second->m_hOriginalController)
		{
			return pawns.second;
		}
	}
	return nullptr;
}

BoneData_t *C_PlayerPawn::GetBone(uint32_t BoneId)
{

	CGameSceneNode *BoneArray = this->m_pGameSceneNode;
	CSkeletonInstance *SkeletonInstance = iHelper->m_Mem.CallVMT<CSkeletonInstance *, 8U>(BoneArray);
	CModelState BoneModelState = SkeletonInstance->modelState;
	BoneData_t *BoneArrayIndex = BoneModelState.bones;

	return BoneArrayIndex + BoneId;
}

C_PlayerPawn *C_PlayerPawn::GetSpectator(std::unordered_map<uint32_t, C_PlayerPawn *> SpectatorMap)
{
	for (auto &spectators : SpectatorMap)
	{
		if (!spectators.second)
			continue;
		if (this->m_hOriginalController == spectators.second->m_hOriginalController)
		{
			
			return spectators.second;
		}
	}
	return nullptr;
}
