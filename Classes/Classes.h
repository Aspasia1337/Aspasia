#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) \
    struct                                  \
    {                                       \
        unsigned char MAKE_PAD(offset);     \
        type name;                          \
    }

#include <iostream>
#include <unordered_map>

#include "../Math/vector.h"
#include "../Helper/helper.h"

namespace terroristBones
{
    enum tBones : DWORD
    {
        pelvis = 0,
        spine_0 = 1,
        spine_1 = 2,
        spine_2 = 3,
        spine_3 = 4,
        neck_0 = 5,
        head_0 = 6,
        clavicle_l = 7,
        arm_upper_l = 8,
        arm_lower_l = 9,
        hand_l = 10,
        weapon_hand_l = 11,
        clavicle_r = 12,
        arm_upper_r = 13,
        arm_lower_r = 14,
        hand_r = 15,
        weapon_hand_r = 16,
        jiggle_primary = 17,
        chesthier_offset = 18,
        weaponhier_jnt = 19,
        weaponhier_r_iktarget = 20,
        weaponhier_l_iktarget = 21,
        leg_upper_l = 22,
        leg_lower_l = 23,
        ankle_l = 24,
        leg_upper_r = 25,
        leg_lower_r = 26,
        ankle_r = 27,
        root_motion = 28,
        leg_l_offset = 29,
        leg_l_iktarget = 30,
        leg_r_offset = 31,
        leg_r_iktarget = 32,
        eyeball_l = 33,
        eyeball_r = 34,
        eye_target = 35,
        head_0_twist = 36,
        finger_middle_meta_l = 37,
        finger_middle_0_l = 38,
        finger_middle_1_l = 39,
        finger_middle_2_l = 40,
        finger_pinky_meta_l = 41,
        finger_pinky_0_l = 42,
        finger_pinky_1_l = 43,
        finger_pinky_2_l = 44,
        finger_index_meta_l = 45,
        finger_index_0_l = 46,
        finger_index_1_l = 47,
        finger_index_2_l = 48,
        finger_thumb_0_l = 49,
        finger_thumb_1_l = 50,
        finger_thumb_2_l = 51,
        finger_ring_meta_l = 52,
        finger_ring_0_l = 53,
        finger_ring_1_l = 54,
        finger_ring_2_l = 55,
        arm_lower_l_twist = 56,
        arm_lower_l_twist1 = 57,
        arm_upper_l_twist1 = 58,
        arm_upper_l_twist = 59,
        pect_l_aimat = 60,
        scapula_l = 61,
        finger_middle_meta_r = 62,
        finger_middle_0_r = 63,
        finger_middle_1_r = 64,
        finger_middle_2_r = 65,
        finger_pinky_meta_r = 66,
        finger_pinky_0_r = 67,
        finger_pinky_1_r = 68,
        finger_pinky_2_r = 69,
        finger_index_meta_r = 70,
        finger_index_0_r = 71,
        finger_index_1_r = 72,
        finger_index_2_r = 73,
        finger_thumb_0_r = 74,
        finger_thumb_1_r = 75,
        finger_thumb_2_r = 76,
        finger_ring_meta_r = 77,
        finger_ring_0_r = 78,
        finger_ring_1_r = 79,
        finger_ring_2_r = 80,
        arm_lower_r_twist = 81,
        arm_lower_r_twist1 = 82,
        arm_upper_r_twist1 = 83,
        arm_upper_r_twist = 84,
        pect_r_aimat = 85,
        scapula_r = 86,
        pect_l_aimup = 87,
        pect_r_aimup = 88,
        scap_aimup = 89,
        pectaim_l = 90,
        pecttrans_l = 91,
        pectaim_r = 92,
        pecttrans_r = 93,
        scap_r_aimat = 94,
        scap_l_aimat = 95,
        pect_l_ptbase = 96,
        pect_r_ptbase = 97,
        ball_l = 98,
        leg_upper_l_twist = 99,
        leg_upper_l_twist1 = 100,
        ball_r = 101,
        leg_upper_r_twist = 102,
        leg_upper_r_twist1 = 103
    };

}

namespace antiTerroristBones
{
    enum ctBones : DWORD
    {
        pelvis = 0,
        spine_0 = 1,
        spine_1 = 2,
        spine_2 = 3,
        spine_3 = 4,
        neck_0 = 5,
        head_0 = 6,
        clavicle_l = 7,
        arm_upper_l = 8,
        arm_lower_l = 9,
        hand_l = 10,
        weapon_hand_l = 11,
        clavicle_r = 12,
        arm_upper_r = 13,
        arm_lower_r = 14,
        hand_r = 15,
        weapon_hand_r = 16,
        jiggle_primary = 17,
        chesthier_offset = 18,
        weaponhier_jnt = 19,
        weaponhier_r_iktarget = 20,
        weaponhier_l_iktarget = 21,
        leg_upper_l = 22,
        leg_lower_l = 23,
        ankle_l = 24,
        leg_upper_r = 25,
        leg_lower_r = 26,
        ankle_r = 27,
        root_motion = 28,
        leg_l_offset = 29,
        leg_l_iktarget = 30,
        leg_r_offset = 31,
        leg_r_iktarget = 32,
        eyeball_l = 33,
        eyeball_r = 34,
        eye_target = 35,
        head_0_twist = 36,
        jiggle_hood = 37,
        finger_middle_meta_l = 38,
        finger_middle_0_l = 39,
        finger_middle_1_l = 40,
        finger_middle_2_l = 41,
        finger_pinky_meta_l = 42,
        finger_pinky_0_l = 43,
        finger_pinky_1_l = 44,
        finger_pinky_2_l = 45,
        finger_thumb_0_l = 46,
        finger_thumb_1_l = 47,
        finger_thumb_2_l = 48,
        finger_index_meta_l = 49,
        finger_index_0_l = 50,
        finger_index_1_l = 51,
        finger_index_2_l = 52,
        finger_ring_meta_l = 53,
        finger_ring_0_l = 54,
        finger_ring_1_l = 55,
        finger_ring_2_l = 56,
        arm_lower_l_twist = 57,
        arm_lower_l_twist1 = 58,
        arm_upper_l_twist = 59,
        arm_upper_l_twist1 = 60,
        pect_l_aimat = 61,
        scapula_l = 62,
        finger_middle_meta_r = 63,
        finger_middle_0_r = 64,
        finger_middle_1_r = 65,
        finger_middle_2_r = 66,
        finger_pinky_meta_r = 67,
        finger_pinky_0_r = 68,
        finger_pinky_1_r = 69,
        finger_pinky_2_r = 70,
        finger_index_meta_r = 71,
        finger_index_0_r = 72,
        finger_index_1_r = 73,
        finger_index_2_r = 74,
        finger_thumb_0_r = 75,
        finger_thumb_1_r = 76,
        finger_thumb_2_r = 77,
        finger_ring_meta_r = 78,
        finger_ring_0_r = 79,
        finger_ring_1_r = 80,
        finger_ring_2_r = 81,
        arm_lower_r_twist = 82,
        arm_lower_r_twist1 = 83,
        arm_upper_r_twist = 84,
        arm_upper_r_twist1 = 85,
        pect_r_aimat = 86,
        scapula_r = 87,
        jiggle_back_micropouches = 88,
        pect_l_aimup = 89,
        pect_r_aimup = 90,
        scap_aimup = 91,
        jiggle_front_micropouches = 92,
        jiggle_front_pouch_01 = 93,
        jiggle_front_pouch_02 = 94,
        jiggle_radio = 95,
        pectaim_l = 96,
        pecttrans_l = 97,
        pectaim_r = 98,
        pecttrans_r = 99,
        scap_r_aimat = 100,
        scap_l_aimat = 101,
        pect_l_ptbase = 102,
        pect_r_ptbase = 103,
        ball_l = 104,
        leg_upper_l_twist = 105,
        leg_upper_l_twist1 = 106,
        jiggle_climbinggear_01 = 107,
        jiggle_climbinggear_02 = 108,
        ball_r = 109,
        leg_upper_r_twist = 110,
        leg_upper_r_twist1 = 111,
        jiggle_holster = 112
    };

};

class Feature
{
public:
    bool *bFeature;
    const char *FeatureName;

    Feature(bool *bFeat, const char *FeatName) : bFeature(bFeat), FeatureName(FeatName) {};
};

class C_BaseEntity
{
public:
    union
    {
        DEFINE_MEMBER_N(uint32_t, m_iHealth, 0x344);
    };
};

struct Color
{
    float x; //  (B)
    float y; //  (G)
    float z; //  (R)
    float w; //  (A)
};

class CMaterial2
{
public:
    virtual const char *GetName() = 0;
    virtual const char *GetShareName() = 0;
};

class CBaseHandle
{
public:
    uint32_t nIndex;
};

class CSceneAnimatableObject
{
public:
    union
    {
        DEFINE_MEMBER_N(CBaseHandle, hOwner, 0xB8);
    };
};

class CMeshData
{
public:
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(CSceneAnimatableObject *, SceneAnimatableObject, 0x18);
        DEFINE_MEMBER_N(CMaterial2 *, CMaterial, 0x20);
        DEFINE_MEMBER_N(byte, colVal, 0x40);
    };
};

class BoneData_t
{
public:
    Vec3 vecPosition;
    float flScale;
};

class CModelState
{
public:
    union
    {
        DEFINE_MEMBER_N(BoneData_t *, bones, 0x80);
    };
};

class CSkeletonInstance
{
public:
    union
    {
        DEFINE_MEMBER_N(CModelState, modelState, 0x170);
    };
};

class CGameSceneNode
{
public:
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(CModelState, m_modelState, 0x170);
        DEFINE_MEMBER_N(Vec3, m_vecOrigin, 0x88);         
        DEFINE_MEMBER_N (bool, m_bDormant, 0xEF);
    };

    //	return iHelper->m_Mem.CallVMT<CSkeletonInstance *, 8> (this);

    // Vec3 GetBoneFromIndex (uint32_t BoneIndex, C_PlayerPawn *pawn) {

    //	CSkeletonInstance *pSkeleton = GetSkeletonInstance ();
    //	if (!pSkeleton)
    //		return *new Vec3 (0, 0, 0);
    //

    //}
};

class C_PlayerController
{
public:
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(uint32_t, m_hPawn, 0x62c);
        DEFINE_MEMBER_N(char, m_iszPlayerName, 0x660);
        DEFINE_MEMBER_N(bool, pawnIsAlive, 0x814);
        DEFINE_MEMBER_N(uint32_t, pawnHealth, 0x818);
        DEFINE_MEMBER_N(uint32_t, m_hOriginalControllerOfCurrentPawn, 0x830);
        DEFINE_MEMBER_N(char *, m_sSanitizedPlayerName, 0x770);              
        DEFINE_MEMBER_N(uint32_t, m_iCompetitiveRanking, 0x790);             
        DEFINE_MEMBER_N(uint32_t, m_iCompetitiveWins, 0x794);                
        DEFINE_MEMBER_N(uint32_t, ping, 0x740);
    };
};

class C_SmokeGrenadeProjectile
{
public:
    union
    {
        DEFINE_MEMBER_N(bool, bDidSmokeEffect, 0x1214);
        DEFINE_MEMBER_N(Vec3, smokeColor, 0x121C);
    };
};

class PostProcessingVolume
{
public:
    union
    {
        DEFINE_MEMBER_N(float, m_flMaxExposure, 0xD58);   
        DEFINE_MEMBER_N(float, m_flMinExposure, 0xD54);   
        DEFINE_MEMBER_N(bool, m_bExposureControl, 0xD6D); 
    };
};

class CBodyComponent
{
    DEFINE_MEMBER_N(CSkeletonInstance *, m_skeletonInstance, 0x50);
};

class CPlayer_CameraServices
{
public:
    union
    {

        DEFINE_MEMBER_N(PostProcessingVolume, m_hActivePostProcessingVolume, 0x1F4); 
    };
};

class CPlayer_ObserverServices
{

public:
    union
    {
        DEFINE_MEMBER_N(uint32_t, m_hObserverTarget, 0x44);
    };
};


class GunEntity {
public:
    union {
        DEFINE_MEMBER_N (char *, weapon_name, 0x20);
    };
};

class C_CSWeaponBase {
public:
    union {
        DEFINE_MEMBER_N (GunEntity *, m_pEntity, 0x10);
    };
};





class C_PlayerPawn
{
public:
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(uint32_t, m_armor, 0x241C);                 
        DEFINE_MEMBER_N(CBodyComponent *, m_CBodyComponent, 0x38);  
        DEFINE_MEMBER_N(CGameSceneNode *, m_pGameSceneNode, 0x328); 
        DEFINE_MEMBER_N(int32_t, pawnHealth, 0x344);
        DEFINE_MEMBER_N(uint8_t, isAlive, 0x348);
        DEFINE_MEMBER_N(bool, pawnIsAlive, 0x814);
        DEFINE_MEMBER_N(uint32_t, m_fFlags, 0x3EC);
        DEFINE_MEMBER_N(uint8_t, m_nActualMoveType, 0x446);
        DEFINE_MEMBER_N(Vec3, vOldOrigin, 0x1324);
        DEFINE_MEMBER_N(char *, playerName, 0x660);
        DEFINE_MEMBER_N(uintptr_t, CGlowProperty, 0xC00);
        DEFINE_MEMBER_N(Color, m_glowColorOverride, 0x40);
        DEFINE_MEMBER_N(bool, m_bGlowing, 0x51);
        DEFINE_MEMBER_N(uint32_t, m_hOriginalController, 0x1508);             
        DEFINE_MEMBER_N(Vec3, m_vecViewOffset, 0xCB0);                        
        DEFINE_MEMBER_N(Vec3, v_angle, 0x124c);                               
        DEFINE_MEMBER_N(CPlayer_CameraServices *, m_pCameraServices, 0x11E0); 
        DEFINE_MEMBER_N(CPlayer_ObserverServices *, m_pObserverServices, 0x11C0);
        DEFINE_MEMBER_N(uint32_t, ObserverEntityHandle, 0x1510); // m_hDetectParentChange TODO MAKE THIS C_PLAYEROBSERVER AND NOT PAWN
        DEFINE_MEMBER_N (C_CSWeaponBase *, m_pClippingWeapon, 0x13A0);
        DEFINE_MEMBER_N (uint8_t , m_iTeamNum, 0x3E3);


    };

    C_PlayerPawn *GetSpectator(std::unordered_map<uint32_t, C_PlayerPawn *> PawnMap);
    C_PlayerPawn *GetPawn(std::unordered_map<uint32_t, C_PlayerPawn *> SpectatorMap);
    BoneData_t *GetBone(uint32_t BoneId);

    bool isInFov = false;
};

class CMsgQAngle
{
public:
    union
    {
        DEFINE_MEMBER_N(Vec3, ViewAngles, 0x18);
    };
};

class CInButtonStatePb
{
public:
    union
    {
        DEFINE_MEMBER_N(uint64_t, nValue, 0x8);
    };
};

class CBaseUserCmdPB
{
public:
    union
    {
        DEFINE_MEMBER_N(CInButtonStatePb *, ButtonState, 0x38);

        DEFINE_MEMBER_N(CMsgQAngle *, CMsgQAngle, 0x40);
    };
};

class CUserCmd
{
public:
    union
    {
        DEFINE_MEMBER_N(CBaseUserCmdPB *, CBaseUserCmdPB, 0x40);
        DEFINE_MEMBER_N(uint32_t, Buttons, 0x60);
        DEFINE_MEMBER_N(uint32_t, ButtonsBackup, 0x68);
    };
};

class ByteColor
{
public:
    unsigned char r, g, b;

    ByteColor(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    bool operator==(const ByteColor ColorA)
    {
        return (ColorA.r == this->r && ColorA.g == this->g && ColorA.b == this->b);
    }
};

class C_EnvSky
{
public:
    union
    {
        DEFINE_MEMBER_N(byte, m_vTintColor, 0xD39);
        DEFINE_MEMBER_N(Color, m_vTintColorLightingOnly, 0xD3D);
        DEFINE_MEMBER_N(float, m_flBrightnessScale, 0xD44);
    };
};

class CCSGOInput
{
public:
    union
    {
        DEFINE_MEMBER_N(Vec3, Angles, 0x3D0);
    };
};

class CAggregateSceneObject
{
public:
    union
    {

        DEFINE_MEMBER_N(byte, lightType, 0xE0);
        DEFINE_MEMBER_N(float, RedColor, 0xE4);
        DEFINE_MEMBER_N(float, GreenColor, 0xE8);
        DEFINE_MEMBER_N(float, BlueColor, 0xEC);
    };
};

class CAggregateSceneObjectDataWorld
{
private:
    char pad_0000[0x38]; // 0x0
public:
    unsigned char r; // 0x38
    unsigned char g; // 0x39
    unsigned char b; // 0x3A
private:
    char pad_0038[0x9];
};

class CViewSetupTRY
{
public:
    union
    {
        DEFINE_MEMBER_N(float, fov, 0x4E8);
        DEFINE_MEMBER_N(float, viewmodel, 0x4d8);
        DEFINE_MEMBER_N(Vec3, viewAngles, 0x4DC);
        DEFINE_MEMBER_N(float, fov2, 0x470);
        DEFINE_MEMBER_N(Vec3, position, 0x4E0);
    };
};

class CAggregateSceneObjectWorld
{
private:
    char pad_0000[0x120];

public:
    int count; // 0x120
private:
    char pad_0120[0x4];

public:
    CAggregateSceneObjectDataWorld *array; // 0x128
};

class Players
{
public:
    C_PlayerPawn *Pawn;
    C_PlayerController *Controller;
    C_PlayerPawn *ObserverPawn;

    Players() : Pawn(nullptr), Controller(nullptr), ObserverPawn(nullptr) {}
    Players(C_PlayerPawn *pawn, C_PlayerController *controller, C_PlayerPawn *observerPawn)
        : Pawn(pawn), Controller(controller), ObserverPawn(observerPawn)
    {
    }
};



class CInterfaceRegister
{
public:
    void *fnCreate;
    const char *szName;
    CInterfaceRegister *pNext;
};

class CustomMaterial_t
{
public:
    CMaterial2 *pMaterial;
    CMaterial2 *pMaterialVisible;

    CustomMaterial_t(CMaterial2 *Material1, CMaterial2 *Material2) : pMaterial(Material1), pMaterialVisible(Material2) {};
};

class CKeyValues3
{
public:
    char pad_0100[0x100];
    std::uint64_t uKey;
    void *pValue;
};

struct KV3ID_t
{
    const char *szName;
    std::uint64_t unk0;
    std::uint64_t unk1;
};