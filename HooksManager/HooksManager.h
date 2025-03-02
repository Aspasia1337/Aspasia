#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include "../math/vector.h"
#include "../Classes/Classes.h"



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



class HooksManager
{
public:

    Vec3 storedViewAngles;
    

	void *CGameInput = nullptr;
	
	HooksManager ( ) {
	}

	bool initHook ( );





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
	



    class OverrideViewClass {
    public:
        typedef void (__fastcall *OverrideViewFunction)(__int64, CViewSetupTRY *);
        static OverrideViewFunction oOverrideViewFunction; //static because shared among all the instances of the class
        static void __fastcall hookOverrideView (__int64 a1, CViewSetupTRY *a2);
    };

    OverrideViewClass m_OverrideViewFunction;
   





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


