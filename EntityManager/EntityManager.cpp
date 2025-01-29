#include "EntityManager.h"



bool EntityManager::populateModule(uintptr_t& module, const char* moduleName) {
	module = reinterpret_cast<uintptr_t>(GetModuleHandle(moduleName));

	if (!module) {
		helper->m_Console.printMessage(INFO, "Cant find module ", moduleName);
		return false;
	}
	helper->m_Console.printMessage(INFO, "Module ", moduleName, " stored");
	return true;
}

bool EntityManager::entityListIterator(std::vector<uintptr_t>& retVector) {

	if (!populateModule(hmoduleClientdll, "client.dll")) {
		return false;
	}

	uintptr_t entityList = *(uintptr_t*)(hmoduleClientdll + 0x1A292F0);

	std::vector<uintptr_t> entities;
	std::vector<uintptr_t> buffer = {};

	for (int i = 0; i < 32; i++) {
		uintptr_t listEntry = *(uintptr_t*)(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));

		if (!listEntry)
			continue;



		uintptr_t entityController = *(uintptr_t*)((listEntry + 120 * (i & 0x1FF))); //0x78

		if (!entityController)
			continue;

		uintptr_t entityControllerPawn = *(uintptr_t*)(entityController + 0x80C);
		if (!entityControllerPawn)
			continue;


		//CBasePlayerPawn
		uintptr_t entity = *(uintptr_t*)(listEntry + 120 * (entityControllerPawn & 0x1FF));

		if (entity) {
			buffer.emplace_back(entity);
		}
	}
	helper->m_Console.printMessage(INFO, "Entity Manager found ", buffer.size(), " entities");
	retVector = buffer;
	return true;
}
