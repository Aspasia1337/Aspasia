#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <type_traits>
#include "../helper/helper.h"


class LocalPlayer
{
private:
	uintptr_t DW_Sensitivity;
public:
	uintptr_t localPlayerPawn;
	int Team;
	int Health;

	uint32_t ShotsFired;
	float AimPunchAngle;

	int m_hPlayerPawn;
	int EntityIndex;
	float Sensitivity;
	int fFlag;
};


class EntityManager
{

private:
	uintptr_t hmoduleClientdll = 0;
	std::vector <uintptr_t> entityVector;


	bool populateModule(uintptr_t& module, const char* moduleName);
public:

	bool entityListIterator(std::vector<uintptr_t>& retVector);
};

inline EntityManager* entManager = new EntityManager();

