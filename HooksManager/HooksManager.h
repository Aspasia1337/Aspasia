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

// Credits for the Union Struct goes to bb110 & pSecurity on UnkownCheats: https://www.unknowncheats.me/forum/c-and-c-/456554-union-struct.html

#include "../Math/vector.h"
#include "../Classes/Classes.h"

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
private:
public:
	Vec3 storedViewAngles;
	static int mutex;

	void *CGameInput = nullptr;

	HooksManager()
	{
	}

	bool initHook();

	// void __fastcall createMove(__int64 *a1, int a2, char a3)
	class CreateMove
	{

	public:
		typedef void(__fastcall *CreateMoveFunction)(CCSGOInput *csgoInput, __int64 nSlot, bool bActivate);
		static CreateMoveFunction oCreateMove;
		static void __fastcall hCreateMove(CCSGOInput *csgoInput, __int64 nSlot, bool bActivate);
		static bool isPlayerInGame(void);
	};
	CreateMove m_CreateMove;

	class CreateMoveTWO
	{

	public:
		typedef void(__fastcall *CreateMoveFunctionTWO)(CCSGOInput *csgoInput, __int64 a2, CUserCmd *a3);
		static CreateMoveFunctionTWO oCreateMoveTWO;
		static void __fastcall hCreateMoveTWO(CCSGOInput *csgoInput, __int64 a2, CUserCmd *a3);
	};
	CreateMoveTWO m_CreateMoveTWO;

	class ValidateInput
	{

	public:
		typedef void(__fastcall *ValidateInputFunction)(CCSGOInput *pInput, int unk);
		static ValidateInputFunction oValidateInput;
		static void __fastcall hValidateInput(CCSGOInput *csgoInput, __int64 a2);
	};

	ValidateInput m_ValidateInput;

	class SetViewAngles
	{
	public:
		typedef void(__fastcall *SetViewAnglesFunction)(__int64 *a1, __int64 a2, Vec3 a3);
		static SetViewAnglesFunction oSetViewAngles;
		static void __fastcall hSetViewAngles(__int64 *a1, __int64 a2, Vec3 a3);
	};

	SetViewAngles m_SetViewAngles;

	class OverrideViewClass
	{
	public:
		typedef void(__fastcall *OverrideViewFunction)(__int64, CViewSetupTRY *);
		static OverrideViewFunction oCameraServices; // This should be shared among all instances
		static void __fastcall hCameraServices(__int64 a1, CViewSetupTRY *a2);
	};

	OverrideViewClass m_OverrideViewFunction;

	class FrameStageNotify
	{
	public:
		typedef int64_t(__fastcall *FrameStageNotifyFunction)(__int64, int);
		static FrameStageNotifyFunction oFrameStageNotify;
		static void __fastcall hFrameStageNotify(__int64 a1, int a2);
	};

	FrameStageNotify m_FrameStageNotify;

	typedef void(__fastcall *calcBonesFunction)(void *a1, unsigned int bone);
	static calcBonesFunction calcBones;

	class OnAddEntity
	{
	public:
		typedef int64_t(__fastcall *OnAddEntityFunction)(__int64 CGameEntitySystem, void *entityPointer, int entityHandle);
		static OnAddEntityFunction oOnAddEntity;
		static void __fastcall hOnAddEntity(__int64 CGameEntitySystem, void *entityPointer, int entityHandle);
	};

	OnAddEntity m_OnAddEntity;

	class OnRemoveEntity
	{
	public:
		typedef int64_t(__fastcall *OnRemoveEntityFunction)(__int64 CGameEntitySystem, void *entityPointer, int entityHandle);
		static OnRemoveEntityFunction oOnRemoveEntity;
		static void __fastcall hOnRemoveEntity(__int64 CGameEntitySystem, void *entityPointer, int entityHandle);
	};

	OnRemoveEntity m_OnRemoveEntity;

	class IsRelativeMouseMode
	{
	public:
		typedef void *(__fastcall *IsRelativeMouseModeFunction)(void *pThisptr, bool bActive);
		static IsRelativeMouseModeFunction oIsRelativeMouseMode;
		static void *__fastcall hIsRelativeMouseFunction(void *pThisptr, bool bActive);

		typedef void *(__fastcall *MouseInputFunction)(void *pThisptr);
		static MouseInputFunction oMouseInput;
		static void __fastcall hMouseInput(void *pThisptr);
	};

	IsRelativeMouseMode m_IsRelativeMouseMode;
};

inline HooksManager *iHooksManager = new HooksManager();
