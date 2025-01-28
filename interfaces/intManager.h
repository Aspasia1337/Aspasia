#pragma once
class InterfacesManger
{
public:
	// __int64 __fastcall CreateInterface(__int64 a1, _DWORD* a2)
	typedef void*(__fastcall* tCreateInterface)(const char* interfaceName,int* returnCode);
	tCreateInterface createInterface;


	InterfacesManger();
	~InterfacesManger();

	BOOL getInterface(uintptr_t &returnInterface, const char* interfaceName);
	void* pGetEntityByIndex(int Index);

};

inline InterfacesManger* intManager = new InterfacesManger();

