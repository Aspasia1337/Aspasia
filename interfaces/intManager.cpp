
#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helper/helper.h"

#include "intManager.h"

void* InterfacesManger::pGetEntityByIndex(int Index)
{
	using fnGetBaseEntity = void* (__thiscall*)(void*, int);
	static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(helper->m_Memory.PatternScanner("client.dll", "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01"));
	return GetBaseEntity(this, Index);
}


BOOL InterfacesManger::getInterface(uintptr_t &returnInterface, const char* interfaceName)
{


	returnInterface = (uintptr_t)createInterface(interfaceName, 0);

	helper->m_Console.printMessage(WARNING, "Interface ", interfaceName, " -> ", returnInterface);
			
	if (returnInterface != 0)
		return true;
}



InterfacesManger::InterfacesManger() {
	helper->m_Console.printMessage(WARNING, "Starting Interfaces Manager");

	createInterface = (tCreateInterface)GetProcAddress(GetModuleHandle("client.dll"), "CreateInterface");

	helper->m_Console.printMessage(WARNING, "Create Interface address: 0x", createInterface);

	uintptr_t testInterface = 0;

	getInterface(testInterface, "ClientToolsInfo_001");

}

InterfacesManger::~InterfacesManger() {

}




