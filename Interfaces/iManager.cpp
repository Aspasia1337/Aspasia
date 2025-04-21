#include "iManager.h"

CInterfaceManager::CInterfaceManager ( )
{
	pGameInput = FindInterface<CGameInput> ("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B DF", "IGameInput", true, 3, 7);
	pPVS = FindInterface<CPVS> ("engine2.dll", "48 8D 0D ? ? ? ? 33 ? FF 50", "CEnginePvs", true, 3, 7);
}

