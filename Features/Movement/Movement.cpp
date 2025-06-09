#include "Movement.h"

#include <iostream>

// https://github.com/pmrowla/hl2sdk-csgo/blob/master/public/const.h#L105
void Movement::BunnyHop(CUserCmd *CUserCmd, C_PlayerPawn *CPlayerPawn)
{
	if (CPlayerPawn->m_fFlags & (1 << 0)) // 65665
	{ 
		CUserCmd->Buttons &= ~(1 << 1);
	}
}
