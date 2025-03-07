#include <iostream>
#include <string>

#include "Visuals.h"
#include "../EntityManager/EntityManager.h"
#include "../includes/imgui/imgui.h"

void Visuals::glowPlayers (uint32_t glowType , float  chamsColor [4])
{
	for (unsigned i = 0; i < iGameEntitySystem->PawnVector.size ( );i++) {

		if (iGameEntitySystem->PawnVector[i] == iGameEntitySystem->LocalPlayerPawn 
			|| iGameEntitySystem->PawnVector[i]->pawnHealth <=0 || iGameEntitySystem->PawnVector[i]->pawnHealth>100)
			continue;

		DWORD colorArgb = ((DWORD)(chamsColor[3] * 255) << 24) |  // Alpha
			((DWORD)(chamsColor[2] * 255) << 16) |  // Blue
			((DWORD)(chamsColor[1] * 255) << 8) |  // Green
			((DWORD)(chamsColor[0] * 255));        // Red

		*(DWORD *)((char *)iGameEntitySystem->PawnVector[i] + 0xC00 + 0x30) = glowType;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
		*(DWORD *)((char *)iGameEntitySystem->PawnVector[i] + 0xC00 + 0x40) = colorArgb;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
		*(DWORD *)((char *)iGameEntitySystem->PawnVector[i] + 0xC00 + 0x51) = 1;
	}
}


void Visuals::renderWithImgui ( ) {

	for (unsigned i = 0; i < iGameEntitySystem->PlayersVector.size ( );i++) {

		Vec3 feetPosition = (iGameEntitySystem->PlayersVector[i]->Pawn->vOldOrigin);

		Vec3 headPos = { feetPosition.x + 0.0f,feetPosition.y + 0.0f,feetPosition.z + 65.0f };

		Vec2 feet, head;

		if (feetPosition.WorldToScreen (feet, iGameEntitySystem->ViewMatrix) && headPos.WorldToScreen (head, iGameEntitySystem->ViewMatrix)) {
			float height = (feet.y - head.y) * 1.5f;
			float width = height / 1.5f;

			char playername[128] = {};

			std::memcpy (playername, &iGameEntitySystem->PlayersVector[i]->Controller->m_iszPlayerName, sizeof (playername));


			// Add health bar

			ImGui::GetBackgroundDrawList ( )->AddRect ({ (feet.x - width),head.y }, { (feet.x - width / 2) + width,head.y + height }, ImColor (255, 255, 255));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y }, ImColor (0, 255, 0), std::to_string (iGameEntitySystem->PlayersVector[i]->Pawn->pawnHealth).c_str ( ));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y - 20 }, ImColor (255, 200, 0), std::to_string (iGameEntitySystem->PlayersVector[i]->Pawn->m_armor).c_str ( ));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y - 40 }, ImColor (0, 150, 255), playername);

			//ImGui::GetBackgroundDrawList()->AddText({ feet.x, feet.y }, ImColor(0, 255, 0), playername);
		}

	}
}


void Visuals::NoSmoke ( ) {
	for (unsigned int i = 0; i < iGameEntitySystem->SmokeGrenadeVector.size ( );i++) {
		iGameEntitySystem->SmokeGrenadeVector[i]->bDidSmokeEffect = true;
		//cgame->SmokeGrenadeVector[i]->smokeColor = *(Vec3*)(new Vec3(globals::smokeColorRGB[0], globals::smokeColorRGB[1], globals::smokeColorRGB[2]));
	}
}

void Visuals::changeSmokeColor (float smokeColor[3])
{
	for (unsigned int i = 0; i < iGameEntitySystem->SmokeGrenadeVector.size ( );i++) {
		iGameEntitySystem->SmokeGrenadeVector[i]->smokeColor = *(Vec3*)(new Vec3 (smokeColor[0], smokeColor[1], smokeColor[2]));
	}

}
