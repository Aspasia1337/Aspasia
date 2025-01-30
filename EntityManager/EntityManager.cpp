#include "EntityManager.h"

struct vec3 {
	float x, y, z;
};

bool EntityManager::populateModule(uintptr_t& module, const char* moduleName) {
	module = reinterpret_cast<uintptr_t>(GetModuleHandle(moduleName));

	if (!module) {
		helper->m_Console.printMessage(INFO, "Cant find module ", moduleName);
		return false;
	}
	helper->m_Console.printMessage(INFO, "Module ", moduleName, " stored");
	return true;
}

bool EntityManager::entityListIterator() {

	if (!populateModule(hmoduleClientdll, "client.dll")) {
		return false;
	}

	listEntryVector.clear();
	controllerVector.clear();
	controllerPawnVector.clear();
	CCSPlayerControllerVector.clear();
	C_BaseEntityVector.clear();

	uintptr_t entityList = *(uintptr_t*)(hmoduleClientdll + 0x1A292F0);

	std::vector<uintptr_t> buffer = {};

	for (int i = 0; i < 64; i++) {
		//uintptr_t listEntry = *(uintptr_t*)(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		uintptr_t listEntryTemp = (entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		uintptr_t listEntry = *(uintptr_t*)listEntryTemp;

		if (!listEntry) {
			continue;
		}

		if ((8 * (i & 0x7FFF) >> 9) == 0) {
			helper->m_Console.printMessage(INFO, "List 1");
		}
		else if ((8 * (i & 0x7FFF) >> 9) == 2) {
			helper->m_Console.printMessage(INFO, "List 2");

		}
		else if ((8 * (i & 0x7FFF) >> 9) == 2) {
			helper->m_Console.printMessage(INFO, "List 3");

		}

		//CSSPlayerController
		uintptr_t entityControllerTemp = ((listEntry + 120 * (i & 0x1FF))); //0x78
		uintptr_t entityController = *(uintptr_t*)entityControllerTemp;


		if (!entityController) {
			continue;
		}

		//C_CSPlayerPawn
		uintptr_t entityControllerPawnTemp = (entityController + 0x80C);
		uintptr_t entityControllerPawn = *(uintptr_t*)entityControllerPawnTemp;



		if (!entityControllerPawn) {
			continue;
		}


		//C_CSPlayerPawn
		uintptr_t entityTemp = (listEntry + 120 * (entityControllerPawn & 0x1FF));
		uintptr_t entity = *(uintptr_t*)entityTemp;

		if (entity) {
			this->listEntryVector.push_back(listEntry);
			this->controllerVector.push_back(entityController);
			this->controllerPawnVector.push_back(entityControllerPawn);
			//this->CCSPlayerControllerVector.push_back((*(CCSPlayerController**)entityControllerTemp));
			this->C_CSPlayerPawnVector.push_back((*(C_CSPlayerPawn**)entityTemp));
			//this - C_BaseEntityVector.push_back((*(C_BaseEntity**)entityControllerTemp));


		}
	}

	helper->m_Console.printMessage(INFO, "Entity Manager found ", this->C_CSPlayerPawnVector.size(), " entities");

	return true;
}


void EntityManager::printPlayerInfo() {
	vec3* position = nullptr;
	for (unsigned i = 0; i < entManager->C_CSPlayerPawnVector.size();i++) {
		helper->m_Console.printMessage(INFO, "Is buyZone? -> ", entManager->C_CSPlayerPawnVector[i]->m_bInBuyZone);

	}
}