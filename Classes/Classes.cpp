#include "Classes.h"

C_PlayerPawn *C_PlayerPawn::GetPawn (std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap)
{
	for (auto &pawns: PawnMap) {
		if (this->m_hOriginalController == pawns.second->m_hOriginalController) {
			return pawns.second;
		}
	}
	return nullptr;
}

C_PlayerPawn *C_PlayerPawn::GetSpectator (std::unordered_map<uint32_t, C_PlayerPawn *> SpectatorMap)
{
	for (auto &spectators : SpectatorMap) {
		if (this->m_hOriginalController == spectators.second->m_hOriginalController) {
			return spectators.second;
		}
	}
	return nullptr;
}


