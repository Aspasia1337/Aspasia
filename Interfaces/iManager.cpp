#include "iManager.h"

CInterfaceManager::CInterfaceManager ( )
{
	pGameInput = FindInterface<CGameInput> ("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B DF", "IGameInput", true, 3, 7);
}

