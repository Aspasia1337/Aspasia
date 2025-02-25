#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <type_traits>

#include "../helper/helper.h"

#include "../math/vector.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


struct Color {
	float x; //  (B)
	float y; //  (G)
	float z; //  (R)
	float w; //  (A)
};

class C_PlayerController {
public:
	union {
		//              Type     Name    Offset
		DEFINE_MEMBER_N (uint32_t, m_hPawn, 0x62c);
		DEFINE_MEMBER_N (char, m_iszPlayerName, 0x660);
		DEFINE_MEMBER_N (bool, pawnIsAlive, 0x814);
		DEFINE_MEMBER_N (uint32_t, pawnHealth, 0x818);
		DEFINE_MEMBER_N (uint32_t, m_hOriginalControllerOfCurrentPawn, 0x830); //linker!

	};
};

class C_SmokeGrenadeProjectile {
public:
	union {
		DEFINE_MEMBER_N (bool, bDidSmokeEffect, 0x1214);
		DEFINE_MEMBER_N (Vec3, smokeColor, 0x121C);

	};
};

class CGameSceneNode {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N (uint32_t, m_modelState, 0x170);   //linker!
    };

};


class CSkeletonInstance {

};

class CBodyComponent {
	DEFINE_MEMBER_N (CSkeletonInstance *, m_skeletonInstance, 0x50);   //linker!

};

class C_PlayerPawn {
public:
	union {
		//              Type     Name    Offset
		DEFINE_MEMBER_N (uint32_t, m_armor, 0x241C);   //linker!
		DEFINE_MEMBER_N (CBodyComponent*, m_CBodyComponent, 0x38);   //linker!
		DEFINE_MEMBER_N (CGameSceneNode*, m_pGameSceneNode, 0x328);   //linker!
		DEFINE_MEMBER_N (int32_t, pawnHealth, 0x344);
		DEFINE_MEMBER_N (uint8_t, isAlive, 0x348);
		DEFINE_MEMBER_N (uint32_t, m_fFlags, 0x3EC);
		DEFINE_MEMBER_N (Vec3, vOldOrigin, 0x1324);
		DEFINE_MEMBER_N (char *, playerName, 0x660);
		DEFINE_MEMBER_N (uintptr_t, CGlowProperty, 0xC00);
		DEFINE_MEMBER_N (Color, m_glowColorOverride, 0x40);
		DEFINE_MEMBER_N (bool, m_bGlowing, 0x51);
		DEFINE_MEMBER_N (uint32_t, m_hOriginalController, 0x1508);   //linker!
	};

	bool isInFov = false;
};


class Players {
public:
	C_PlayerPawn *Pawn;
	C_PlayerController *Controller;

	Players ( ) : Pawn (nullptr), Controller (nullptr) {}
	Players (C_PlayerPawn *pawn, C_PlayerController *controller)
		: Pawn (pawn), Controller (controller) {
	}
};


class GameEntitySystem {
public:
	uintptr_t pEntityList;
	uintptr_t clientDll;
	uintptr_t engine2Dll;
	DWORD pMaxIndex;
	std::vector<C_PlayerController *> ControllerVector;
	std::vector<C_PlayerPawn *> PawnVector;
	std::vector<Players *> PlayersVector;
	std::vector<C_SmokeGrenadeProjectile *> SmokeGrenadeVector;
	float (*ViewMatrix)[4][4];
	C_PlayerPawn *LocalPlayerPawn;

	C_PlayerController *LocalPlayerController;
	int indexes = 0;

	GameEntitySystem ( ) {
		init ( );
	}

	void init ( ) {
		clientDll = (uintptr_t)GetModuleHandle ("client.dll");
		pEntityList = (uintptr_t)GetModuleHandle ("client.dll") + (uintptr_t)0x1A359B0;
		pMaxIndex = *(DWORD *)(*(uintptr_t *)(clientDll + (uintptr_t)0x1B5C6C8) + (uintptr_t)0x20F0);
		ViewMatrix = reinterpret_cast<float(*)[4][4]>(iHelper->m_Mem.ResolveRip (iHelper->m_Mem.PatternScanner ("client.dll", "48 8D ?? ?? ?? ?? ?? 48 C1 E0 06 48 03 C1 C3 CC CC"), 3, 7));
		engine2Dll = (uintptr_t)GetModuleHandle ("engine2.dll");
	}

	std::string GetSchemaName (void *entity);

	void *GetEntityByIndexFunction (int Index);

	void getGameEntities ( );

	void getAllPlayers ( );

	void getClosetEnemis ( );

	void getEnemisByFov ( );

};

inline GameEntitySystem *iGameEntitySystem = new GameEntitySystem ( );
