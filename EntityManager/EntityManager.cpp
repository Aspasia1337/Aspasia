#include "EntityManager.h"
#include "../math/vector.h"
#include "../includes/imgui/imgui.h"

#include <iostream>
#include <string>

std::string GameEntitySystem::GetSchemaName(void* entity)
{
	const uintptr_t entity_identity = *(uintptr_t*)((uintptr_t)entity + 0x10);
	if (!entity_identity)return "";

	const uintptr_t entity_class_info = *(uintptr_t*)(entity_identity + 0x8);
	if (!entity_class_info)return "";

	const uintptr_t schema_class_info_data = *(uintptr_t*)(entity_class_info + 0x30);
	if (!schema_class_info_data)return "";

	const char* class_name = *(const char**)(schema_class_info_data + 0x8);
	if (!class_name) return "";

	char buffer[1024];

	lstrcpyA(buffer, class_name);
	return std::string(buffer);
}

void* GameEntitySystem::GetEntityByIndexFunction(int Index)
{
	// sub_606B10 IDA returns the entity
	using fnGetBaseEntity = uintptr_t * (__thiscall*)(void*, int);
	static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(helper->m_Mem.PatternScanner("client.dll", "81 FA ? ? ? ? 77 36 8B C2 C1 F8 09 83 F8 3F 77 2C 48 98 48 8B 4C C1 ? 48 85 C9 74 20 8B C2 25 ? ? ? ? 48 6B C0 78 48 03 C8 74 10 8B 41 10 25 ? ? ? ? 3B C2 75 04 48 8B 01 C3"));
	return GetBaseEntity(*(uintptr_t**)pEntityList, Index);
}


void GameEntitySystem::getGameEntities ( ) {

	pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1A359C0) + (uintptr_t)0x20F0);

	ControllerVector.clear ( );
	PawnVector.clear ( );

	//helper->m_Console.printMessage(WARNING,"-----------------------------------");

	for (unsigned int i = 0; i < pMaxIndex; i++) {

		void *Entity = (void *)(GetEntityByIndexFunction (i));
		if (!Entity)
			continue;

		// helper->m_Console.printMessage(DEBUG, "Found! - ", GetSchemaName(Entity));

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
	}

}

void GameEntitySystem::getAllPlayers(){

	getGameEntities ( );
	PlayersVector.clear ( );

	for (unsigned int i = 0; i < PawnVector.size ( ); i++) {
		for (unsigned int j = 0; j < ControllerVector.size ( ); j++) {
			if (PawnVector[i]->m_hOriginalController == ControllerVector[j]->m_hOriginalControllerOfCurrentPawn) {
				PlayersVector.emplace_back (new Players (PawnVector[i], ControllerVector[j]));
				break;
			}
		}
	}
}


void GameEntitySystem::glowPatch()
{

	C_PlayerPawn* aspasiaUser = (C_PlayerPawn*)(clientDll + 0x1889F30);

	Color glowColor = { 0.2f, 1.0f, 0.2f, 1.0f };

	for (unsigned i = 0; i < PawnVector.size();i++) {

		if ((char*)PawnVector[i] == *(char**)aspasiaUser)
			continue;

		DWORD colorArgb = ((DWORD)(glowColor.w * 255) << 24) |
			((DWORD)(glowColor.z * 255) << 16) |
			((DWORD)(glowColor.y * 255) << 8) |
			((DWORD)(glowColor.x * 255));

		*(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x40) = colorArgb;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
		*(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x51) = 1;
	}
}


void GameEntitySystem::renderDot() {
	float(*ViewMatrix)[4][4] = (float(*)[4][4])(clientDll + 0x1AA17C0);

	for (unsigned i = 0; i < PlayersVector.size();i++) {
		Vec3 feetPosition = (PlayersVector[i]->Pawn->vOldOrigin);

		Vec3 headPos = { feetPosition.x + 0.0f,feetPosition.y + 0.0f,feetPosition.z + 65.0f };

		Vec2 feet, head;

		if (feetPosition.WorldToScreen(feet, ViewMatrix) && headPos.WorldToScreen(head, ViewMatrix)) {
			float height = (feet.y - head.y) * 1.5f;
			float width = height / 1.5f;

			//char playername[128] = {};

			//std::memcpy(playername, PlayersVector[i]->Controller->m_iszPlayerName, sizeof(playername));



			//ImGui::GetBackgroundDrawList()->AddRect({ (feet.x-width/2),head.y }, {(feet.x-width/2) + width,head.y+height}, ImColor(255,255,255));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x, feet.y }, ImColor (0, 150, 0), std::to_string (PlayersVector[i]->Pawn->pawnHealth).c_str ( ));
			ImGui::GetBackgroundDrawList()->AddText({ feet.x, feet.y-30 }, ImColor(0, 0, 80), std::to_string(PlayersVector[i]->Pawn->m_armor).c_str());
			//ImGui::GetBackgroundDrawList()->AddText({ feet.x, feet.y }, ImColor(0, 255, 0), playername);
		}
		
	}
}