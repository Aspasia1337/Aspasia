#pragma once

#include "../../Classes/Classes.h"
#include "../../globals.h"

#include <unordered_map>

class Misc
{
public:
	class Spectators
	{
	public:
		void ShowSpectatorList(std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap, std::unordered_map<uint32_t, C_PlayerController *> ControllersMap,
							   std::unordered_map<uint32_t, C_PlayerPawn *> ObserverMap, C_PlayerPawn *LocalPlayerPawn, C_PlayerController *LocalPlayerController);
	};
	Spectators m_Spectators;

	class ClantagChanger
	{
	public:
		static void(__fastcall *SetClantag)(const char *string);
	};
	ClantagChanger m_Clantag;

	class ScopeRemoval
	{
	public:
		typedef void *(__fastcall *RemoveScopeFN)(void *a1, void *a2);
		static RemoveScopeFN oRemoveScopeFn;
		static void hRemoveScope(void *a1, void *a2);
	};
	ScopeRemoval m_ScopeRemoval;
};

inline Misc *iMisc = new Misc();
