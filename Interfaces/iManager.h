#pragma once

#include "Windows.h"
#include "vector"
#include "iostream"

#include "../Helper/helper.h"

class CGameInput {
public:
};

class CPVS {
public:
        // TODO: Not needed for now.
};

class CInterfaceManager
{
public:

    CGameInput *pGameInput;
    CPVS *pPVS;

    CInterfaceManager ( );

    void *GetInterface (const char *mod, const char *iface, void *&gIface)
    {
        using fn_capture_iface_t = void *(*)(const char *, int *);
        auto fn_addr = (fn_capture_iface_t)GetProcAddress (GetModuleHandleA (mod), "CreateInterface");
        if (!fn_addr)
        {
            iHelper->m_Console.printMessage(WARNING,"Failed to get CreateInterface from", mod);
            return nullptr;
        }
        void *iface_addr = fn_addr (iface, nullptr);

        if (iface_addr == nullptr)
        {
            iHelper->m_Console.printMessage (WARNING, "Failed to Find", mod);
        }
        else
            iHelper->m_Console.printMessage (WARNING, "Interface ", mod, "found!");

        gIface = iface_addr;

        return gIface;
    }

    template <typename T>
    T *FindInterface (const char *moduleName, const char *pattern, const char *interfaceName, bool useRip = false, int offset = 0, int extra = 0) {
        uint8_t *address = iHelper->m_Mem.PatternScanner(moduleName, pattern);
        if (!address) {
            iHelper->m_Console.printMessage (WARNING, "Failed to find address for", interfaceName);
            return nullptr;
        }

        T *resolvedAddress = nullptr;
        if (useRip) {
            resolvedAddress = *reinterpret_cast<T **>(iHelper->m_Mem.ResolveRip(address, offset, extra));
        }
        else {
            resolvedAddress = *reinterpret_cast<T **>(address + offset);
        }

        if (!resolvedAddress) {
            iHelper->m_Console.printMessage (WARNING, "Failed to resolve address for", interfaceName);
        }
        else {
            iHelper->m_Console.printMessage(DEBUG,"Found", interfaceName, "at", resolvedAddress);
        }

        return resolvedAddress;
    }

    static void (__fastcall *setPVS)(CPVS *, bool);
};

inline CInterfaceManager *g_pInterfaces = new CInterfaceManager ( );
