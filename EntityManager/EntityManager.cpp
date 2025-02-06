#include "EntityManager.h"


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


void GameEntitySystem::getGameEntities() {
	pMaxIndex = *(DWORD*)(*(uintptr_t*)(clientDll + (uintptr_t)0x1A359C0) + (uintptr_t)0x20F0);

	ControllerVector.clear();
	PawnVector.clear();

	//helper->m_Console.printMessage(WARNING,"-----------------------------------");

	for (unsigned int i = 0; i < pMaxIndex; i++) {

		void* Entity = (void*)(GetEntityByIndexFunction(i));
		if (!Entity)
			continue;

		// helper->m_Console.printMessage(DEBUG, "Found! - ", GetSchemaName(Entity));

		if (GetSchemaName(Entity) == ("C_CSPlayerPawnBase")) {

			C_PlayerPawn* Pawn = (C_PlayerPawn*)Entity;

			if (Pawn->pawnHealth > 0 && Pawn->pawnHealth <= 100 && Pawn->isAlive == 0) {
				PawnVector.push_back(Pawn);
				continue;
			}
		}

		if (GetSchemaName(Entity) == ("CBasePlayerController")) {

			C_PlayerController* Controller = (C_PlayerController*)Entity;

			if (Controller->pawnIsAlive)
			{
				ControllerVector.push_back(Controller);
				continue;
			}
		}
	}
	helper->m_Console.printMessage(DEBUG, "EntityManager found ", ControllerVector.size(), " controllers & ", PawnVector.size(), " pawns!");
}


void GameEntitySystem::glowPatch()
{
	Color glowColor = { 1.0f, 0.0f, 0.0f, 1.0f };

	for (unsigned i = 1; i < PawnVector.size();i++) {

		DWORD colorArgb = ((DWORD)(glowColor.w * 255) << 24) |
			((DWORD)(glowColor.z * 255) << 16) |
			((DWORD)(glowColor.y * 255) << 8) |
			((DWORD)(glowColor.x * 255));

		*(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x40) = colorArgb;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
		*(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x51) = 1;
	}
}
