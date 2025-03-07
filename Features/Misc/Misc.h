#pragma once

#include "../../Classes/Classes.h"
#include "../../globals.h"

class Misc
{
public:

	class Spectators {
	public:
		void ShowSpectatorList (std::vector<Players *> PlayersVector, C_PlayerPawn *LocalPlayerPawn, C_PlayerController *LocalPlayerController);
	};

	Spectators m_Spectators;
};

inline Misc *iMisc = new Misc ( );

