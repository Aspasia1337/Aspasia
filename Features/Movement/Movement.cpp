#include "Movement.h"

//FLAGS REFS -> https://github.com/pmrowla/hl2sdk-csgo/blob/master/public/const.h#L105
void Movement::BunnyHop (CUserCmd *CUserCmd, C_PlayerPawn *CPlayerPawn)
{
	if (CPlayerPawn->m_fFlags & 257 && CUserCmd->Buttons & (1 << 1)) {
	CUserCmd->Buttons &= ~(1 << 1);
	}
}
