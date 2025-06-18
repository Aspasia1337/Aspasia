#include "EntityManager.h"

std::vector<C_EnvSky *> Visual::CEnvSkyVector;

std::string GameEntitySystem::GetSchemaName(void *entity)
{
	const uintptr_t entity_identity = *(uintptr_t *)((uintptr_t)entity + 0x10);
	if (!entity_identity)
		return "";

	const uintptr_t entity_class_info = *(uintptr_t *)(entity_identity + 0x8);
	if (!entity_class_info)
		return "";

	const char *distinguisher = *(char **)(entity_identity + 0x20);
	if (!distinguisher)
		return "";

	if (strcmp(distinguisher, "c_cs_observer_for_precache") == 0)
	{
		return "c_cs_observer_for_precache";
	}

	if (strcmp(distinguisher, "env_sky") == 0)
	{
		return "env_sky";
	}

	if (strcmp (distinguisher, "chicken") == 0)
	{
		return "chicken";
	}
	

	const uintptr_t schema_class_info_data = *(uintptr_t *)(entity_class_info + 0x30);
	if (!schema_class_info_data)
		return "";

	const char *class_name = *(const char **)(schema_class_info_data + 0x8);
	if (!class_name)
		return "";

	char buffer[1024];

	lstrcpyA(buffer, class_name);
	
	//iHelper->m_Console.printMessage (WARNING, buffer);

	return std::string(buffer);
}

uint32_t GameEntitySystem::GetEntityHandle(void *entity)
{
	const uintptr_t entity_identity = *(uintptr_t *)((uintptr_t)entity + 0x10);
	if (!entity_identity)
		return 0;

	const uint32_t *entityHandle = (uint32_t *)(entity_identity + 0x10);
	if (!entityHandle)
		return 0;

	return *(uint32_t *)entityHandle;
}

void *GameEntitySystem::GetEntityByIndexFunction(int Index)
{
	// sub_606B10 check on IDA
	using fnGetBaseEntity = uintptr_t *(__thiscall *)(void *, int);
	static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(iHelper->m_Mem.PatternScanner("client.dll", "81 FA ? ? ? ? 77 36 8B C2 C1 F8 09 83 F8 3F 77 2C 48 98 48 8B 4C C1 ? 48 85 C9 74 20 8B C2 25 ? ? ? ? 48 6B C0 78 48 03 C8 74 10 8B 41 10 25 ? ? ? ? 3B C2 75 04 48 8B 01 C3"));
	return GetBaseEntity(*(uintptr_t **)pEntityList, Index);
}

void GameEntitySystem::getGameEntities()
{
	pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1B23888) + (uintptr_t)0x20F0);

	ControllerVector.clear();
	PawnVector.clear();
	SmokeGrenadeVector.clear();
	CBaseEntityVector.clear();
	ObserverPawnVector.clear();
	Visual::CEnvSkyVector.clear();

	for (unsigned int i = 0; i < pMaxIndex; i++)
	{

		void *Entity = (void *)(GetEntityByIndexFunction(i));

		if (!Entity)
			continue;

		auto name = GetSchemaName(Entity);

		if (GetSchemaName(Entity) == ("C_CSPlayerPawnBase"))
		{
			C_PlayerPawn *Pawn = (C_PlayerPawn *)Entity;
			PawnMap.insert(std::make_pair(GetEntityHandle(Entity), Pawn));
		}

		if (GetSchemaName(Entity) == ("CBasePlayerController"))
		{
			C_PlayerController *Controller = (C_PlayerController *)Entity;
			ControllerMap.insert(std::make_pair(GetEntityHandle(Entity), Controller));
		}

		// This means player is an Observer ~ Useful for Spectators
		if (GetSchemaName(Entity) == "c_cs_observer_for_precache")
		{
			C_PlayerPawn *Pawn = (C_PlayerPawn *)Entity;
			ObserverMap.insert(std::make_pair(GetEntityHandle(Entity), Pawn));
		}

		if (GetSchemaName(Entity) == ("C_BaseCSGrenadeProjectile"))
		{
			C_SmokeGrenadeProjectile *SmokeProjectile = (C_SmokeGrenadeProjectile *)Entity;
			SmokeGrenadeVector.push_back(SmokeProjectile);
		}

		if (GetSchemaName(Entity) == ("C_BaseEntity"))
		{
			C_BaseEntity *BaseEntity = (C_BaseEntity *)Entity;
			CBaseEntityVector.push_back(BaseEntity);
		}

		if (GetSchemaName(Entity) == ("env_sky"))
		{
			C_EnvSky *SkyEnt = (C_EnvSky *)Entity;
			Visual::CEnvSkyVector.push_back(SkyEnt);
		}

	}
}

// Takes every player Alive & Dead
void GameEntitySystem::getAllPlayers()
{
	getGameEntities();
	PlayersMap.clear();

	C_PlayerController *localPlayerController = *(C_PlayerController **)(clientDll + 0x1A4E870);

	for (unsigned int i = 0; i < PawnVector.size(); i++)
	{
		for (unsigned int j = 0; j < ControllerVector.size(); j++)
		{
			if (PawnVector[i]->m_hOriginalController == ControllerVector[j]->m_hOriginalControllerOfCurrentPawn)
			{
				for (int k = 0; k < ObserverPawnVector.size(); k++)
				{
					if (PawnVector[i]->m_hOriginalController == ObserverPawnVector[k]->m_hOriginalController)
					{
						if (ControllerVector[j] == localPlayerController)
						{
							LocalPlayerPawn = PawnVector[i];
							LocalPlayerController = ControllerVector[j];
							LocalPlayerObserver = ObserverPawnVector[k];
							continue;
						}
						PlayersMap.emplace_back(new Players(PawnVector[i], ControllerVector[j], ObserverPawnVector[k]));
						break;
					}
				}
			}
		}
	}
}

void GameEntitySystem::getClosetEnemis()
{
	getAllPlayers();

	// TODO: bubble sort but could be imporved with std::sort 
	for (unsigned int i = 0; i < PlayersMap.size() - 1; i++)
	{
		for (unsigned int j = 0; j < PlayersMap.size() - 1 - i; j++)
		{
			if (CalculateDistance(LocalPlayerPawn->vOldOrigin, PlayersMap[j]->Pawn->vOldOrigin) >
				CalculateDistance(LocalPlayerPawn->vOldOrigin, PlayersMap[j + 1]->Pawn->vOldOrigin))
			{
				std::swap(PlayersMap[j], PlayersMap[j + 1]);
			}
		}
	}
}

void GameEntitySystem::getEnemisByFov()
{
	Vec3 *viewangles = (Vec3 *)(iGameEntitySystem->clientDll + 0x1A733C0);
	float distance;

	if (!viewangles)
		return;

	getAllPlayers();

	if (PlayersMap.size() < 1)
		return;

	for (unsigned int i = 0; i < PlayersMap.size() - 1; i++)
	{
		for (unsigned j = 0; j < PlayersMap.size() - 1 - i; j++)
		{

			Vec3 playerToMe = Vec3(
				PlayersMap[j]->Pawn->m_pGameSceneNode->m_vecOrigin.x + PlayersMap[j]->Pawn->m_vecViewOffset.x - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.x + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.x,
				PlayersMap[j]->Pawn->m_pGameSceneNode->m_vecOrigin.y + PlayersMap[j]->Pawn->m_vecViewOffset.y - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.y + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.y,
				PlayersMap[j]->Pawn->m_pGameSceneNode->m_vecOrigin.z + PlayersMap[j]->Pawn->m_vecViewOffset.z - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.z + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.z);
			Vec3 playerToMeTwo = Vec3(
				PlayersMap[j + 1]->Pawn->m_pGameSceneNode->m_vecOrigin.x + PlayersMap[j + 1]->Pawn->m_vecViewOffset.x - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.x + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.x,
				PlayersMap[j + 1]->Pawn->m_pGameSceneNode->m_vecOrigin.y + PlayersMap[j + 1]->Pawn->m_vecViewOffset.y - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.y + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.y,
				PlayersMap[j + 1]->Pawn->m_pGameSceneNode->m_vecOrigin.z + PlayersMap[j + 1]->Pawn->m_vecViewOffset.z - LocalPlayerPawn->m_pGameSceneNode->m_vecOrigin.z + iGameEntitySystem->LocalPlayerPawn->m_vecViewOffset.z);

			double hyp = sqrtf(playerToMe.x * playerToMe.x + playerToMe.y * playerToMe.y);
			float targetX = (float)(atan(playerToMe.z / hyp) * (180.0 / std::numbers::pi));			 // Pitch
			float targetY = (float)(atan2(playerToMe.y, playerToMe.x) * (180.0 / std::numbers::pi)); // Yaw

			double hyp2 = sqrtf(playerToMeTwo.x * playerToMeTwo.x + playerToMeTwo.y * playerToMeTwo.y);
			float targetXTWO = (float)(atan(playerToMeTwo.z / hyp2) * (180.0 / std::numbers::pi));			  // Pitch
			float targetYTWO = (float)(atan2(playerToMeTwo.y, playerToMeTwo.x) * (180.0 / std::numbers::pi)); // Yaw

			bool x = false, y = false;

			if (IsTargetWithinFOV(targetX, targetY, viewangles->x, viewangles->y, Globals::aimbotFov, distance))
			{
				PlayersMap[j]->Pawn->isInFov = true;
				x = true;
			}
			else
			{
				PlayersMap[j]->Pawn->isInFov = false;
			}
			if (IsTargetWithinFOV(targetXTWO, targetYTWO, viewangles->x, viewangles->y, Globals::aimbotFov, distance))
			{
				PlayersMap[j + 1]->Pawn->isInFov = true;
				y = true;
			}
			else
			{
				PlayersMap[j + 1]->Pawn->isInFov = false;
			}

			if (!x && y)
			{
				std::swap(PlayersMap[j], PlayersMap[j + 1]);
			}

			else if (x && y)
			{

				float angleDiff1 = fabs(targetX - viewangles->x) + fabs(targetY - viewangles->y);
				float angleDiff2 = fabs(targetXTWO - viewangles->x) + fabs(targetYTWO - viewangles->y);

				if (angleDiff2 < angleDiff1)
				{
					std::swap(PlayersMap[j], PlayersMap[j + 1]);
				}
			}
		}
	}
}

C_PlayerPawn *GameEntitySystem::GetPlayerPawn()
{
	C_PlayerPawn *LocalPlayerPawn;

	LocalPlayerPawn = *(C_PlayerPawn **)(iGameEntitySystem->clientDll + 0x18540D0);

	if (!LocalPlayerPawn)
		return nullptr;

	return LocalPlayerPawn;
}

C_PlayerController *GameEntitySystem::GetPlayerController()
{
	C_PlayerController *LocalPlayerController;
	LocalPlayerController = *(C_PlayerController **)(iGameEntitySystem->clientDll + 0x1A4E870);

	if (!LocalPlayerController)
		return nullptr;

	return LocalPlayerController;
}

C_PlayerController *GameEntitySystem::GetControllerFromPawn(C_PlayerPawn *Pawn)
{
	for (auto &controller : ControllerMap)
	{
		if (controller.second->m_hOriginalControllerOfCurrentPawn == Pawn->m_hOriginalController)
			return controller.second;
	}

	return nullptr;
}

C_PlayerPawn *GameEntitySystem::GetPawnFromObserver(C_PlayerPawn *Observer)
{
	for (auto &pawn : PawnMap)
	{
		if (pawn.second->m_hOriginalController == Observer->m_hOriginalController)
			return pawn.second;
	}

	return nullptr;
}
