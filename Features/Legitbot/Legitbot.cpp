#include "Legitbot.h"

#include <iostream>
#include "../../Helper/helper.h"

Vec3 Legitbot::GetBonePosition::GetBonePosFromIndex(uint32_t index, C_PlayerPawn *Player, C_PlayerPawn *Me)
{
	if (Player == Me)
		return *new Vec3(0, 0, 0);

	CGameSceneNode *BoneArray = Player->m_pGameSceneNode;
	CSkeletonInstance *SkeletonInstance = iHelper->m_Mem.CallVMT<CSkeletonInstance *, 8U>(BoneArray);
	CModelState BoneModelState = SkeletonInstance->modelState;
	BoneData_t *BoneArrayWHAT = BoneModelState.bones;

	BoneArrayWHAT += index;
	return (BoneArrayWHAT->vecPosition);
}
