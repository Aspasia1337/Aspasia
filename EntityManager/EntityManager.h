#include <vector>
#include <type_traits>
#include <algorithm>
#include <string>

#include "../Helper/helper.h"
#include "../Classes/Classes.h"
#include "../globals.h"
#include "../Math/vector.h"
#include "../Includes/imgui/imgui.h"

#include "../Features/Visuals/Visual.h"

#include <unordered_map>

class GameEntitySystem
{
public:
	uintptr_t pEntityList;
	uintptr_t clientDll;
	uintptr_t engine2Dll;
	DWORD pMaxIndex;
	std::vector<C_BaseEntity *> CBaseEntityVector;
	std::vector<C_PlayerController *> ControllerVector;
	std::vector<C_PlayerPawn *> PawnVector;
	std::vector<C_PlayerPawn *> ObserverPawnVector;
	std::vector<Players *> PlayersMap;
	std::vector<C_SmokeGrenadeProjectile *> SmokeGrenadeVector;

	std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap;
	std::unordered_map<uint32_t, C_PlayerPawn *> ObserverMap;
	std::unordered_map<uint32_t, C_PlayerController *> ControllerMap;
	// std::vector<C_EnvSky *> CEnvSkyVector;

	float (*ViewMatrix)[4][4];
	C_PlayerPawn *LocalPlayerPawn;
	C_PlayerPawn *Pawn;
	Vec3 *viewangles;

	C_PlayerController *LocalPlayerController;
	C_PlayerPawn *LocalPlayerObserver;
	int indexes = 0;

	GameEntitySystem()
	{
		init();
	}

	void init()
	{
		// Offsets
		clientDll = (uintptr_t)GetModuleHandle("client.dll");
		pEntityList = (uintptr_t)GetModuleHandle("client.dll") + (uintptr_t)0x19FFE48;
		pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1B23888) + (uintptr_t)0x20F0);
		ViewMatrix = reinterpret_cast<float(*)[4][4]>(iHelper->m_Mem.ResolveRip(iHelper->m_Mem.PatternScanner("client.dll", "48 8D ?? ?? ?? ?? ?? 48 C1 E0 06 48 03 C1 C3 CC CC"), 3, 7));
		engine2Dll = (uintptr_t)GetModuleHandle("engine2.dll");
		viewangles = (Vec3 *)(clientDll + 0x1A733C0);
	}

	std::string GetSchemaName(void *entity);

	uint32_t GetEntityHandle(void *entity);

	void *GetEntityByIndexFunction(int Index);

	void getGameEntities();

	void getAllPlayers();

	void getClosetEnemis();

	void getEnemisByFov();

	C_PlayerPawn *GetPlayerPawn();

	C_PlayerController *GetPlayerController();

	C_PlayerController *GetControllerFromPawn(C_PlayerPawn *Pawn);

	C_PlayerPawn *GetPawnFromObserver(C_PlayerPawn *Observer);
};

inline GameEntitySystem *iGameEntitySystem = new GameEntitySystem();
