#include "EntityManager.h"
#include "../math/vector.h"
#include "../includes/imgui/imgui.h"

#include <iostream>
#include <string>

std::string GameEntitySystem::GetSchemaName (void *entity)
{
	const uintptr_t entity_identity = *(uintptr_t *)((uintptr_t)entity + 0x10);
	if (!entity_identity)return "";

	const uintptr_t entity_class_info = *(uintptr_t *)(entity_identity + 0x8);
	if (!entity_class_info)return "";

	const uintptr_t schema_class_info_data = *(uintptr_t *)(entity_class_info + 0x30);
	if (!schema_class_info_data)return "";

	const char *class_name = *(const char **)(schema_class_info_data + 0x8);
	if (!class_name) return "";

	char buffer[1024];

	lstrcpyA (buffer, class_name);
	return std::string (buffer);
}

void *GameEntitySystem::GetEntityByIndexFunction (int Index)
{
	// sub_606B10 IDA returns the entity
	using fnGetBaseEntity = uintptr_t * (__thiscall *)(void *, int);
	static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(helper->m_Mem.PatternScanner ("client.dll", "81 FA ? ? ? ? 77 36 8B C2 C1 F8 09 83 F8 3F 77 2C 48 98 48 8B 4C C1 ? 48 85 C9 74 20 8B C2 25 ? ? ? ? 48 6B C0 78 48 03 C8 74 10 8B 41 10 25 ? ? ? ? 3B C2 75 04 48 8B 01 C3"));
	return GetBaseEntity (*(uintptr_t **)pEntityList, Index);
}


void GameEntitySystem::getGameEntities ( ) {

	pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1A359C0) + (uintptr_t)0x20F0);

	ControllerVector.clear ( );
	PawnVector.clear ( );
	SmokeGrenadeVector.clear ( );

	for (unsigned int i = 0; i < pMaxIndex; i++) {

		void *Entity = (void *)(GetEntityByIndexFunction (i));
		if (!Entity)
			continue;


		if (GetSchemaName (Entity) == ("C_CSPlayerPawnBase")) {

			C_PlayerPawn *Pawn = (C_PlayerPawn *)Entity;

			if (Pawn->pawnHealth > 0 && Pawn->pawnHealth <= 100 && Pawn->isAlive == 0) {
				PawnVector.push_back (Pawn);
				continue;
			}
		}

		if (GetSchemaName (Entity) == ("CBasePlayerController")) {

			C_PlayerController *Controller = (C_PlayerController *)Entity;

			if (Controller->pawnIsAlive)
			{
				ControllerVector.push_back (Controller);
				continue;
			}
		}


		//Schema

		if (GetSchemaName (Entity) == ("C_BaseCSGrenadeProjectile")) {
			C_SmokeGrenadeProjectile *SmokeProjectile = (C_SmokeGrenadeProjectile *)Entity;
			SmokeGrenadeVector.push_back (SmokeProjectile);

		}
	}

}

void GameEntitySystem::getAllPlayers ( ) {

	getGameEntities ( );
	PlayersVector.clear ( );

	// Change this to qsort 

	for (unsigned int i = 0; i < PawnVector.size ( ); i++) {
		for (unsigned int j = 0; j < ControllerVector.size ( ); j++) {
			if (PawnVector[i]->m_hOriginalController == ControllerVector[j]->m_hOriginalControllerOfCurrentPawn) {
				PlayersVector.emplace_back (new Players (PawnVector[i], ControllerVector[j]));
				break;
			}
		}
	}

}






void GameEntitySystem::noFlash ( ) {

	float *flashtime = (float *)0x00000290467B1478;
	float *flahscreenshot = (float *)0x00000290467B147C;

	*(float *)flashtime = 0;
	*(float *)flahscreenshot = 0;
}


