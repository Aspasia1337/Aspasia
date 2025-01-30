#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <type_traits>

#include "../helper/helper.h"

#define STR_MERGE_IMPL(a, b) a##b // Combine a & b = ab
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class CEntitySystem {

};

class CGameEntitySystem : CEntitySystem {

};

class CEntityInstance {

};

class C_BaseEntity : CEntityInstance {
	union {
		DEFINE_MEMBER_N(int, health, 0x344);
	};
};

class CBasePlayerController : C_BaseEntity {
public:

};


class CCSPlayerController : CBasePlayerController {
public:
	union {
		DEFINE_MEMBER_N(char*, playerName, 0x660);
		DEFINE_MEMBER_N(bool, hasHelmet, 0x821);
	};
};


class C_BasePlayerPawn : CCSPlayerController {

};

class C_CSPlayerPawnBase : C_BasePlayerPawn {

};


class C_CSPlayerPawn : C_CSPlayerPawnBase {
public:
	union {
		DEFINE_MEMBER_N(bool, m_bInBuyZone, 0x1580);
	};

};





class EntityManager
{

private:
	uintptr_t hmoduleClientdll = 0;
	std::vector <uintptr_t> listEntryVector;
	std::vector <uintptr_t> controllerVector;
	std::vector <uintptr_t> controllerPawnVector;
	std::vector <uintptr_t> entityVector;
	std::vector <CCSPlayerController*> CCSPlayerControllerVector;
	std::vector <C_CSPlayerPawn*> C_CSPlayerPawnVector;
	std::vector <C_BaseEntity*> C_BaseEntityVector;

	bool populateModule(uintptr_t& module, const char* moduleName);


public:

	bool entityListIterator();


	void printPlayerInfo();

};

inline EntityManager* entManager = new EntityManager();