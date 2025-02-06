#include <Windows.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <type_traits>

#include "../helper/helper.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

struct vec3 {
    float x, y, z;
};



class C_PlayerController {
public:
    union {
        DEFINE_MEMBER_N(uint32_t, m_hPawn, 0x62c);
        DEFINE_MEMBER_N(bool, pawnIsAlive, 0x814);
        DEFINE_MEMBER_N(uint32_t, pawnHealth, 0x818);
    };
};


class C_PlayerPawn {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(int32_t, pawnHealth, 0x344);
        DEFINE_MEMBER_N(uint8_t, isAlive, 0x348);
        DEFINE_MEMBER_N(vec3, vOldOrigin, 0x1324);
        DEFINE_MEMBER_N(char**, playerName, 0x660);
        DEFINE_MEMBER_N(uint32_t, m_hController, 0x133C);
    };
};


class CGameEntitySystem {

private:
    uintptr_t pEntityList;
    uintptr_t clientDll;
    DWORD pMaxIndex;
    std::vector<C_PlayerController*> ControllerVector;
    std::vector<C_PlayerPawn*> PawnVector;
    int indexes = 0;

public:
    
    CGameEntitySystem() {
        init();
    }              

    void init() {
        clientDll = (uintptr_t)GetModuleHandle("client.dll");
        pEntityList = (uintptr_t)GetModuleHandle("client.dll") + (uintptr_t)0x1B5C6D8;
        pMaxIndex = *(DWORD*)(*(uintptr_t*)(clientDll + (uintptr_t)0x1A359C0) + (uintptr_t)0x20F0);
    }


    std::string GetSchemaName(void* entity)
    {
        const uintptr_t entity_identity = *(uintptr_t*)((uintptr_t)entity + 0x10);
        if (!entity_identity)return "";

        const uintptr_t entity_class_info = *(uintptr_t*)(entity_identity + 0x8);
        if (!entity_class_info)return "";
        const uintptr_t schema_class_info_data = *(uintptr_t*)(entity_class_info + 0x30);
        if (!schema_class_info_data)return "";
        const char* class_name = *(const char**)(schema_class_info_data + 0x8);
        if (!class_name) return "";
        char buffer[1024];
        lstrcpyA(buffer, class_name);
        return std::string(buffer);
    }

    void* GetEntityByIndexFunction(int Index)
    {
        // sub_606B10 IDA returns the entity
        using fnGetBaseEntity = uintptr_t * (__thiscall*)(void*, int);
        static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(helper->m_Mem.PatternScanner("client.dll", "81 FA ? ? ? ? 77 36 8B C2 C1 F8 09 83 F8 3F 77 2C 48 98 48 8B 4C C1 ? 48 85 C9 74 20 8B C2 25 ? ? ? ? 48 6B C0 78 48 03 C8 74 10 8B 41 10 25 ? ? ? ? 3B C2 75 04 48 8B 01 C3"));
        return GetBaseEntity(*(uintptr_t**)pEntityList, Index);
    }

    void getActualEntities() {
        pMaxIndex = *(DWORD*)(*(uintptr_t*)(clientDll + (uintptr_t)0x1A359C0) + (uintptr_t)0x20F0);

        ControllerVector.clear();
        PawnVector.clear();

        helper->m_Console.printMessage(WARNING,"-----------------------------------");

        for (unsigned int i = 0; i < pMaxIndex; i++) {

            void* Entity = (void*)(GetEntityByIndexFunction(i));
            if (!Entity)
                continue;

           // helper->m_Console.printMessage(DEBUG, "Found! - ", GetSchemaName(Entity));

            if (GetSchemaName(Entity) == ("C_CSPlayerPawnBase")) {
               
                C_PlayerPawn* Pawn = (C_PlayerPawn*)Entity;
                
                if (Pawn->pawnHealth > 0 && Pawn->pawnHealth <= 100 && Pawn->isAlive == 0) {
                    PawnVector.push_back(Pawn);
                    continue;
                }
            }

            if (GetSchemaName(Entity) == ("CBasePlayerController")) {

                C_PlayerController* Controller = (C_PlayerController*)Entity;

                if (Controller->pawnIsAlive)
                {
                    ControllerVector.push_back(Controller);
                    continue;
                }
                
            }
            
        }

        helper->m_Console.printMessage(DEBUG, "EntityManager found ", ControllerVector.size(), " controllers & ", PawnVector.size(), " pawns!");
    }

    void glowPatch()
    {

        struct Color {
            float x; // Azul (B)
            float y; // Verde (G)
            float z; // Rojo (R)
            float w; // Alfa (A)
        };

        constexpr std::ptrdiff_t m_bGlowing = 0x51;
        constexpr std::ptrdiff_t m_glowColorOverride = 0x40;
        constexpr std::ptrdiff_t m_Glow = 0xC00;


        Color glowColor = { 1.0f, 0.0f, 0.0f, 1.0f };  // Azul completamente opaco

        for (unsigned i = 1; i < PawnVector.size();i++) {

            DWORD colorArgb = ((DWORD)(glowColor.w * 255) << 24) |
                ((DWORD)(glowColor.z * 255) << 16) |
                ((DWORD)(glowColor.y * 255) << 8) |
                ((DWORD)(glowColor.x * 255));

            *(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x40) = colorArgb;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
            *(DWORD*)((char*)PawnVector[i] + 0xC00 + 0x51) = 1;
        }
    }
};

inline CGameEntitySystem* cgame = new CGameEntitySystem();
