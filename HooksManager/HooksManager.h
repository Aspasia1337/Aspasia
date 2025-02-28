#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include "../math/vector.h"




//https://www.unknowncheats.me/forum/counter-strike-2-a/687526-bones-indexes.html  checkkkkk bopnes
enum class BoneIDs
{
    Head = 6,
    Neck = 5,
    UpperChest = 4,
    LowerChest = 3,
    Stomach = 2,
    Pelivs = 0,

    LeftShoulder = 8,
    LeftElbow = 9,
    LeftArm = 10,
    RightShoulder = 13,
    RightElbow = 14,
    RightArm = 15,

    LeftThigh = 22,
    LeftKnee = 23,
    LeftLeg = 30,
    LeftFoot = 98,
    RightThigh = 25,
    RightKnee = 26,
    RightLeg = 32,
    RightFoot = 101,

    LeftPinky1 = 41,
    LeftPinky2 = 42,
    LeftPinky3 = 43,
    LeftPinky4 = 44,

    LeftRing1 = 52,
    LeftRing2 = 53,
    LeftRing3 = 54,
    LeftRing4 = 55,

    LeftMiddle1 = 37,
    LeftMiddle2 = 38,
    LeftMiddle3 = 39,
    LeftMiddle4 = 40,

    LeftIndex1 = 45,
    LeftIndex2 = 46,
    LeftIndex3 = 47,
    LeftIndex4 = 48,

    LeftThumb1 = 49,
    LeftThumb2 = 50,
    LeftThumb3 = 51,

    RightPinky1 = 66,
    RightPinky2 = 67,
    RightPinky3 = 68,
    RightPinky4 = 69,

    RightRing1 = 77,
    RightRing2 = 78,
    RightRing3 = 79,
    RightRing4 = 80,

    RightMiddle1 = 62,
    RightMiddle2 = 63,
    RightMiddle3 = 64,
    RightMiddle4 = 65,

    RightIndex1 = 70,
    RightIndex2 = 71,
    RightIndex3 = 72,
    RightIndex4 = 73,

    RightThumb1 = 74,
    RightThumb2 = 75,
    RightThumb3 = 76,
};

/*
class CUserCmd
{
public:
    MEM_PAD(0x8); // 0x0 VTABLE
    MEM_PAD(0x10); // TODO: find out what this is, added 14.08.2024
    CCSGOUserCmdPB csgoUserCmd; // 0x18
    CInButtonState nButtons; // 0x58
    MEM_PAD(0x20); // 0x78
*/






class CCSGOInput {
public:
    union {
        DEFINE_MEMBER_N (Vec3, angles, 0x3D0);

    };
};


class CMsgQAngle {
public:
    union {
        DEFINE_MEMBER_N (Vec3, angles, 0x18);
    };

};

class qAngles {
public:
    union {
        DEFINE_MEMBER_N (CMsgQAngle, msgqangle, 0x80);
    };

};

class PBaseUserCmd {
public:
    union {
        DEFINE_MEMBER_N (qAngles, qangles, 0x40);
    };

};

class CUserCmd {
public:
    union{
        DEFINE_MEMBER_N (PBaseUserCmd, baseusercmd, 0x40);
        DEFINE_MEMBER_N (uint32_t, buttons, 0x60);
    };

};



class CAggregateSceneObject
{
public:
    unsigned char pad_0[0xE4];
    float red;
    float green;
    float blue;
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


class HooksManager
{
public:

    Vec3 storedViewAngles;
    

	void *CGameInput = nullptr;
	
	HooksManager ( ) {
	}

	bool initHook ( );


	bool CreateHook (uint8_t *targetAddress, void *hookFunction, void **originalFunction, const char *hookName);

	// void __fastcall RenderSmoke(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6)
	class SmokeEffect {
	public:
		typedef void (__fastcall *RenderSmokeParticlesFunction)(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
		static RenderSmokeParticlesFunction oRenderSmokeParticles;
		static void __fastcall hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
	};
	SmokeEffect m_SmokeEffect;

	class FlashEffect {
	public:
		typedef void (__fastcall *FlashEffectFunction)(__int64 a1, __int64 a2, float* a3);
		static FlashEffectFunction oFlashEffect;
		static void __fastcall hFlashEffect (__int64 a1, __int64 a2, float *a3);
	};
	FlashEffect m_FlashEffect;

	//void __fastcall createMove(__int64 *a1, int a2, char a3)
	class CreateMove {

	public:
		typedef void (__fastcall *CreateMoveFunction)(CCSGOInput * csgoInput, __int64 nSlot, bool bActivate);
		static CreateMoveFunction oCreateMove;
		static void __fastcall hCreateMove (CCSGOInput *csgoInput, __int64 nSlot, bool bActivate);
        bool isPlayerInGame (void);


	};
	CreateMove m_CreateMove;


    class CreateMoveTWO {

    public:
        typedef void (__fastcall *CreateMoveFunctionTWO)(CCSGOInput *csgoInput, __int64 a2, CUserCmd *a3);
        static CreateMoveFunctionTWO oCreateMoveTWO;
        static void __fastcall hCreateMoveTWO (CCSGOInput *csgoInput, __int64 a2, CUserCmd *a3);


    };
    CreateMoveTWO m_CreateMoveTWO;



    class ValidateInput {

    public:


        typedef void (__fastcall *ValidateInputFunction)(CCSGOInput *pInput, int unk);
        static ValidateInputFunction oValidateInput;
        static void __fastcall hValidateInput (CCSGOInput *csgoInput, __int64 a2);
    };

    ValidateInput m_ValidateInput;


	class SetViewAngles {
	public:

		typedef void (__fastcall *SetViewAnglesFunction)(__int64 *a1, __int64 a2, Vec3 a3);
		static SetViewAnglesFunction oSetViewAngles;
		static void __fastcall hSetViewAngles (__int64 *a1, __int64 a2, Vec3 a3);

	};

	SetViewAngles m_SetViewAngles;
	

	class DrawObjectClass {
	public:
		typedef void (__fastcall *DrawObjectFunction)(void *a1, void *a2, void *a3, int a4, void *a5, void *a6, void *a7, void *a8);
		static DrawObjectFunction oDrawObject;
		static void __fastcall hDrawObject (void *a1, void *a2, void *a3, int a4, void *a5, void *a6, void *a7, void *a8);
	};

	DrawObjectClass m_DrawObject;

    class OverrideViewClass {
    public:
        typedef void (__fastcall *OverrideViewFunction)(__int64, CViewSetupTRY *);
        static OverrideViewFunction oOverrideViewFunction; //static because shared among all the instances of the class
        static void __fastcall hookOverrideView (__int64 a1, CViewSetupTRY *a2);
    };

    OverrideViewClass m_OverrideViewFunction;
   


    class LightningModulation {
    public:

        typedef void *(__fastcall *LightningModulationFunction)(__int64 a1, CAggregateSceneObject *a2, __int64 a3); 
        static LightningModulationFunction oLightningModulation;
        static void* __fastcall hLightningModulation (__int64 a1, CAggregateSceneObject *a2, __int64 a3);
    };
    LightningModulation m_LightningModulation;


    class WorldModulation
    {
    public:
        typedef void *(__fastcall *oModulateWorldColorFn)(CAggregateSceneObjectWorld *, void *);
        static oModulateWorldColorFn oModulateWorldColor;
        static void *__fastcall hModulateWorldColor (CAggregateSceneObjectWorld *pAggregateSceneObject, void *a2);
    };

    WorldModulation m_WorldModulation;

    typedef void (__fastcall *calcBonesFunction)(void* a1, unsigned int bone);

    static calcBonesFunction calcBones;





};

inline HooksManager *iHooksManager = new HooksManager ( );


