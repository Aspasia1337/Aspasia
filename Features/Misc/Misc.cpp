#include "Misc.h"

void Misc::Spectators::ShowSpectatorList (std::vector<Players *> PlayersVector, C_PlayerPawn *LocalPlayerPawn, C_PlayerController *LocalPlayerController)
{
	Globals::Spectators.clear ( );
	for (int i = 0; i < PlayersVector.size ( );i++) {
		if (LocalPlayerPawn->pawnHealth <= 0) {
			continue;
		}
		if (PlayersVector[i]->Pawn->pawnHealth == 0 && PlayersVector[i]->ObserverPawn->m_pObserverServices->m_hObserverTarget == LocalPlayerController->m_hPawn) {
			Globals::Spectators.push_back (PlayersVector[i]->Controller->m_sSanitizedPlayerName);
		}
	}
}
