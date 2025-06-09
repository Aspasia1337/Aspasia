#include "AntiAim.h"
#include "../../globals.h"

// Correct angle from actual view, without this
// angles are referenced to the center of the map and not players point of view
void AntiAim::CorrectionFromView(Vec3 &Angle, Vec3 PlayerAngles)
{
	float TempJaw = Globals::AAJaw - fabs(PlayerAngles.y);
	float TempPitch = Globals::AAPitch - fabs(PlayerAngles.x);

	if (PlayerAngles.y > 0)
	{
		Angle.y = -TempJaw;
	}
	else
	{
		Angle.y = TempJaw;
	}

	if (PlayerAngles.x > 0)
	{
		//Angle.x = -TempPitch;
		Angle.x = 88.9f;
	}
	else
	{
		//Angle.x = TempPitch;
		Angle.x = 88.9f;
	}
}

void AntiAim::OnMove(CCSGOInput *CCSGOInput, CUserCmd *UserCmd)
{
	// Save player angles, mandatory for moving camera arround when calling ValidateInput()
	PlayerAngles = UserCmd->CBaseUserCmdPB->CMsgQAngle->ViewAngles;

	// Override angles with Anti Aim Angles
	Vec3 TempAngles = *new Vec3(Globals::AAPitch, Globals::AAJaw, 0.0f);
	CorrectionFromView(TempAngles, PlayerAngles);
	UserCmd->CBaseUserCmdPB->CMsgQAngle->ViewAngles = TempAngles;
}

void AntiAim::ValidateInput::hValidateInput(CCSGOInput *CSGOInput, __int64 a2)
{
	if (Globals::AntiAim || Globals::ManualAA)
	{
		// Save Anti Aim Angles
		Vec3 SavedAngles = CSGOInput->Angles;

		// Before calling oValidate override angles with camera angles
		CSGOInput->Angles = PlayerAngles;

		// Call Validate camera angles
		oValidateInput(CSGOInput, a2);

		// Restore Anti Aim Angles
		CSGOInput->Angles = SavedAngles;
	}
	else
	{
		return oValidateInput(CSGOInput, a2);
	}
}