#pragma once

#include "../../globals.h"
#include "../../Classes/Classes.h"

class Movement
{
public:
	void BunnyHop(CUserCmd *CUserCmd, C_PlayerPawn *CPlayerPawn);
};
inline Movement *iMovement = new Movement();
