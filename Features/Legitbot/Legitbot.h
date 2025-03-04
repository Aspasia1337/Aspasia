#pragma once
#include "../../math/vector.h"

class Legitbot
{
public:

	class GetBonePosition {
	public:

		void ( __fastcall *GetBonePositionFunction)(void* player, uint32_t index, Vec3 &position, Vec3 &rotation);

	};
	GetBonePosition m_BonePosition;


};

inline Legitbot *iLegitBot = new Legitbot ( );

