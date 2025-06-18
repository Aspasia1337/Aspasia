#include "Misc.h"

void Misc::Spectators::ShowSpectatorList(std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap, std::unordered_map<uint32_t, C_PlayerController *> ControllersMap,
										 std::unordered_map<uint32_t, C_PlayerPawn *> ObserverMap, C_PlayerPawn *LocalPlayerPawn, C_PlayerController *LocalPlayerController)
{
	std::vector<std::string> TempSpectators; // To avoid flickering on ImGui

	for (const auto &obs : ObserverMap)
	{
		if (LocalPlayerPawn->pawnHealth <= 0 || (obs.second->GetPawn(PawnMap))->pawnHealth > 0)
		{
			continue;
		}
		if (obs.second->m_pObserverServices->m_hObserverTarget == LocalPlayerController->m_hPawn)
		{
			for (const auto &controller : ControllersMap)
			{
				if (controller.second->m_hOriginalControllerOfCurrentPawn == obs.second->m_hOriginalController && controller.second != LocalPlayerController)
					TempSpectators.push_back(controller.second->m_sSanitizedPlayerName);
			}
		}
		//	Globals::Spectators.push_back (ControllersMap[obs.first]->second->m_sSanitizedPlayerName); 
	}

	Globals::Spectators = TempSpectators;
}

void Misc::ScopeRemoval::hRemoveScope(void *a1, void *a2)
{
	if (Globals::RemoveScope)
		return;
	oRemoveScopeFn(a1, a2);
}
