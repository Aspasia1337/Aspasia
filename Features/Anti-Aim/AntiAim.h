#pragma once
#include "../../HooksManager/HooksManager.h"
class AntiAim
{
public:
	float velocidad = 0.0f;
	static Vec3 PlayerAngles; // Store Player Angles
	void OnMove(CCSGOInput *CCSGOInput, CUserCmd *UserCmd);
	void CorrectionFromView(Vec3 &Angle, Vec3 PlayerAngles);
	class ValidateInput
	{
	public:
		typedef void(__fastcall *ValidateInputFunction)(CCSGOInput *CSGOInput, int unk);
		static ValidateInputFunction oValidateInput;
		static void hValidateInput(CCSGOInput *CSGOInput, __int64 a2);
	};
	ValidateInput mValidateInput;
};
inline AntiAim *iAntiAim = new AntiAim();
