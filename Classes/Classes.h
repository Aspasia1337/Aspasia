#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include "../math/vector.h"

class C_BaseEntity {
public:
    union {
        DEFINE_MEMBER_N (uint32_t, m_iHealth, 0x344);
    };
};

struct Color {
    float x; //  (B)
    float y; //  (G)
    float z; //  (R)
    float w; //  (A)
};

class CMaterial2 {
public:

};

class CBaseHandle {
public:
    union {
        DEFINE_MEMBER_N (uint32_t, nIndex, 0xD0);
    };
};

class CSceneAnimatableObject
{
public:    
    union {
        DEFINE_MEMBER_N (CBaseHandle, hOwner, 0xB0);
    };
};



class CMeshData {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N (CSceneAnimatableObject *, SceneAnimatableObject, 0x18);   
        DEFINE_MEMBER_N (CMaterial2*, CMaterial, 0x20);
        DEFINE_MEMBER_N (CMaterial2*, CMaterialCopy, 0x28);
    };
};


class CGameSceneNode {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N (uint32_t, m_modelState, 0x170);   //linker!
        DEFINE_MEMBER_N (Vec3, m_vecOrigin, 0x88);   //linker!
    };

};


class C_PlayerController {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N (uint32_t, m_hPawn, 0x62c);
        DEFINE_MEMBER_N (char, m_iszPlayerName, 0x660);
        DEFINE_MEMBER_N (bool, pawnIsAlive, 0x814);
        DEFINE_MEMBER_N (uint32_t, pawnHealth, 0x818);
        DEFINE_MEMBER_N (uint32_t, m_hOriginalControllerOfCurrentPawn, 0x830); //linker!
        DEFINE_MEMBER_N (char*, m_sSanitizedPlayerName, 0x770); //linker!

    };
};



class C_SmokeGrenadeProjectile {
public:
    union {
        DEFINE_MEMBER_N (bool, bDidSmokeEffect, 0x1214);
        DEFINE_MEMBER_N (Vec3, smokeColor, 0x121C);
    };
};





class PostProcessingVolume {
public:
    union {
        DEFINE_MEMBER_N (float, m_flMaxExposure, 0xD58);   //linker!
        DEFINE_MEMBER_N (float, m_flMinExposure, 0xD54);   //linker!
        DEFINE_MEMBER_N (bool, m_bExposureControl, 0xD6D);   //linker!


    };


};


class CSkeletonInstance {

};


class CBodyComponent {
    DEFINE_MEMBER_N (CSkeletonInstance *, m_skeletonInstance, 0x50);   //linker!

};


class CPlayer_CameraServices {
public:
    union {

        DEFINE_MEMBER_N (PostProcessingVolume, m_hActivePostProcessingVolume, 0x1F4);   //linker!


    };
};

class CPlayer_ObserverServices {

public:
    union {
        DEFINE_MEMBER_N (uint32_t, m_hObserverTarget, 0x44);
    };
};

class C_PlayerPawn {
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N (uint32_t, m_armor, 0x241C);   //linker!
        DEFINE_MEMBER_N (CBodyComponent *, m_CBodyComponent, 0x38);   //linker!
        DEFINE_MEMBER_N (CGameSceneNode *, m_pGameSceneNode, 0x328);   //linker!
        DEFINE_MEMBER_N (int32_t, pawnHealth, 0x344);
        DEFINE_MEMBER_N (uint8_t, isAlive, 0x348);
        DEFINE_MEMBER_N (uint32_t, m_fFlags, 0x3EC);
        DEFINE_MEMBER_N (uint8_t, m_nActualMoveType, 0x446);
        DEFINE_MEMBER_N (Vec3, vOldOrigin, 0x1324);
        DEFINE_MEMBER_N (char *, playerName, 0x660);
        DEFINE_MEMBER_N (uintptr_t, CGlowProperty, 0xC00);
        DEFINE_MEMBER_N (Color, m_glowColorOverride, 0x40);
        DEFINE_MEMBER_N (bool, m_bGlowing, 0x51);
        DEFINE_MEMBER_N (uint32_t, m_hOriginalController, 0x1508);   //linker!
        DEFINE_MEMBER_N (Vec3, m_vecViewOffset, 0xCB0);   //linker!
        DEFINE_MEMBER_N (Vec3, v_angle, 0x124c);   //linker!
        DEFINE_MEMBER_N (CPlayer_CameraServices *, m_pCameraServices, 0x11E0);   //linker!
        DEFINE_MEMBER_N (CPlayer_ObserverServices*, m_pObserverServices, 0x11C0);
    

    };

    bool isInFov = false;
};



class CMsgQAngle {
public:
    union {
        DEFINE_MEMBER_N (Vec3, ViewAngles, 0x18);
    };

};

class CInButtonStatePb {
public:
    union {
    };
};

class CBaseUserCmdPB {
public:
    union {
        DEFINE_MEMBER_N (CMsgQAngle *, CMsgQAngle, 0x40);

    };
};





class CUserCmd {
public:
    union {
        DEFINE_MEMBER_N (CBaseUserCmdPB *, CBaseUserCmdPB, 0x40);
        DEFINE_MEMBER_N (uint32_t, Buttons, 0x60);
        DEFINE_MEMBER_N (uint32_t, ButtonsBackup, 0x68);


    };

};


class CCSGOInput {
public:
    union {
        DEFINE_MEMBER_N (Vec3, Angles, 0x3D0);

    };
};


class CAggregateSceneObject
{
public:
    union {

        DEFINE_MEMBER_N (byte, lightType, 0xE0);
        DEFINE_MEMBER_N (float, RedColor, 0xE4);
        DEFINE_MEMBER_N (float, GreenColor, 0xE8);
        DEFINE_MEMBER_N (float, BlueColor, 0xEC);
    };
};

class CAggregateSceneObjectDataWorld {
private:
    char pad_0000[0x38]; // 0x0
public:
    unsigned char r; // 0x38
    unsigned char g; // 0x39
    unsigned char b; // 0x3A
private:
    char pad_0038[0x9];
};


class CViewSetupTRY {
public:
    union {
        DEFINE_MEMBER_N (float, fov, 0x4E8);
        DEFINE_MEMBER_N (float, viewmodel, 0x4d8);
        DEFINE_MEMBER_N (Vec3, viewAngles, 0x4DC);
        DEFINE_MEMBER_N (float, fov2, 0x470);
        DEFINE_MEMBER_N (Vec3, position, 0x4E0);


    };

};

class CAggregateSceneObjectWorld {
private:
    char pad_0000[0x120];
public:
    int count; // 0x120
private:
    char pad_0120[0x4];
public:
    CAggregateSceneObjectDataWorld *array; // 0x128
};
