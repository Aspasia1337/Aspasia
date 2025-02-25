#include "EntityManager.h"
#include "../math/vector.h"
#include "../includes/imgui/imgui.h"
#include "../globals.h"
#include <algorithm> 
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
	static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(iHelper->m_Mem.PatternScanner ("client.dll", "81 FA ? ? ? ? 77 36 8B C2 C1 F8 09 83 F8 3F 77 2C 48 98 48 8B 4C C1 ? 48 85 C9 74 20 8B C2 25 ? ? ? ? 48 6B C0 78 48 03 C8 74 10 8B 41 10 25 ? ? ? ? 3B C2 75 04 48 8B 01 C3"));
	return GetBaseEntity (*(uintptr_t **)pEntityList, Index);
}


void GameEntitySystem::getGameEntities ( ) {

	pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1B5C6C8) + (uintptr_t)0x20F0);

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

		if (GetSchemaName (Entity) == ("C_BaseCSGrenadeProjectile")) {
			C_SmokeGrenadeProjectile *SmokeProjectile = (C_SmokeGrenadeProjectile *)Entity;
			SmokeGrenadeVector.push_back (SmokeProjectile);
		}
	}

}



void GameEntitySystem::getAllPlayers ( ) {

	getGameEntities ( );
	PlayersVector.clear ( );

	C_PlayerPawn* localPlayerPawn = *(C_PlayerPawn**)(clientDll + 0x1889F20);

	for (unsigned int i = 0; i < PawnVector.size ( ); i++) {
		for (unsigned int j = 0; j < ControllerVector.size ( ); j++) {
			if (PawnVector[i]->m_hOriginalController == ControllerVector[j]->m_hOriginalControllerOfCurrentPawn) {
				if (PawnVector[i] == localPlayerPawn) {
					LocalPlayerPawn = PawnVector[i];
					LocalPlayerController = ControllerVector[i];
					continue;
				}
				PlayersVector.emplace_back (new Players (PawnVector[i], ControllerVector[j]));
				break;
			}
		}
	}

}

void GameEntitySystem::getClosetEnemis ( )
{
	getAllPlayers ( );
	
	//bubble sort
	for (unsigned int i = 0; i < PlayersVector.size ( ) - 1;i++) {
		for (unsigned int j = 0; j < PlayersVector.size ( ) - 1 - i; j++) {
			if (CalculateDistance (LocalPlayerPawn->vOldOrigin, PlayersVector[j]->Pawn->vOldOrigin) > 
				CalculateDistance (LocalPlayerPawn->vOldOrigin, PlayersVector[j+1]->Pawn->vOldOrigin)) {
				std::swap (PlayersVector[j], PlayersVector[j + 1]);
			}
		}
	}

}



void GameEntitySystem::getEnemisByFov ( ) {
	Vec3 *viewangles = (Vec3 *)(iGameEntitySystem->clientDll + 0x1AABA40);
	float distance;

	if (!viewangles) return; 

	getAllPlayers ( ); 

	if (PlayersVector.size ( ) < 1) return; // Evita el desbordamiento


	for (unsigned int i = 0; i < PlayersVector.size ( ) - 1; i++) {
		for (unsigned j = 0; j < PlayersVector.size ( ) - 1 - i; j++) {

			Vec3 playerToMe = Vec3 (
				PlayersVector[j]->Pawn->vOldOrigin.x - LocalPlayerPawn->vOldOrigin.x,
				PlayersVector[j]->Pawn->vOldOrigin.y - LocalPlayerPawn->vOldOrigin.y,
				PlayersVector[j]->Pawn->vOldOrigin.z - LocalPlayerPawn->vOldOrigin.z);

			Vec3 playerToMeTwo = Vec3 (
				PlayersVector[j + 1]->Pawn->vOldOrigin.x - LocalPlayerPawn->vOldOrigin.x,
				PlayersVector[j + 1]->Pawn->vOldOrigin.y - LocalPlayerPawn->vOldOrigin.y,
				PlayersVector[j + 1]->Pawn->vOldOrigin.z - LocalPlayerPawn->vOldOrigin.z);

			double hyp = sqrtf (playerToMe.x * playerToMe.x + playerToMe.y * playerToMe.y);
			float targetX = (float)(atan (playerToMe.z / hyp) * (180.0 / std::numbers::pi)); // Pitch
			float targetY = (float)(atan2 (playerToMe.y, playerToMe.x) * (180.0 / std::numbers::pi)); // Yaw

			double hyp2 = sqrtf (playerToMeTwo.x * playerToMeTwo.x + playerToMeTwo.y * playerToMeTwo.y);
			float targetXTWO = (float)(atan (playerToMeTwo.z / hyp2) * (180.0 / std::numbers::pi)); // Pitch
			float targetYTWO = (float)(atan2 (playerToMeTwo.y, playerToMeTwo.x) * (180.0 / std::numbers::pi)); // Yaw

			bool x = false, y = false;

			if (IsTargetWithinFOV (targetX, targetY, viewangles->x, viewangles->y, globals::aimbotFov, distance)) {
				PlayersVector[j]->Pawn->isInFov = true;
				x = true;
			}
			else {
				PlayersVector[j]->Pawn->isInFov = false;

			}
			if (IsTargetWithinFOV (targetXTWO, targetYTWO, viewangles->x, viewangles->y, globals::aimbotFov, distance)) {
				PlayersVector[j+1]->Pawn->isInFov = true;
				y = true;
			}
			else {
				PlayersVector[j + 1]->Pawn->isInFov = false;

			}

			if (!x && y) {
				std::swap (PlayersVector[j], PlayersVector[j + 1]);
			}
			else if (x && y) {

				float angleDiff1 = fabs (targetX - viewangles->x) + fabs (targetY - viewangles->y);
				float angleDiff2 = fabs (targetXTWO - viewangles->x) + fabs (targetYTWO - viewangles->y);

				if (angleDiff2 < angleDiff1) {
					std::swap (PlayersVector[j], PlayersVector[j + 1]);
				}
			}
		}
	}
}

