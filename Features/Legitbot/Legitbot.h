#pragma once
#include "../../Math/vector.h"
#include "../../Classes/Classes.h"

class Legitbot
{
public:
	class GetBonePosition
	{
	public:
		void(__fastcall *GetBonePositionFunction)(void *player, uint32_t index, Vec3 &position, Vec3 &rotation);
		Vec3 GetBonePosFromIndex(uint32_t index, C_PlayerPawn *Player, C_PlayerPawn *Me);
	};
	GetBonePosition m_BonePosition;
};

inline Legitbot *iLegitBot = new Legitbot();
