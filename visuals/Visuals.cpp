#include <iostream>
#include <string>

#include "Visuals.h"
#include "../EntityManager/EntityManager.h"
#include "../includes/imgui/imgui.h"


void Visuals::glowPlayers ( )
{
	C_PlayerPawn *aspasiaUser = (C_PlayerPawn *)(cgame->clientDll + 0x1889F30);

	Color glowColor = { 0.2f, 1.0f, 0.2f, 1.0f };

	for (unsigned i = 0; i < cgame->PawnVector.size ( );i++) {

		if ((char *)cgame->PawnVector[i] == *(char **)aspasiaUser)
			continue;

		DWORD colorArgb = ((DWORD)(glowColor.w * 255) << 24) |
			((DWORD)(glowColor.z * 255) << 16) |
			((DWORD)(glowColor.y * 255) << 8) |
			((DWORD)(glowColor.x * 255));

		*(DWORD *)((char *)cgame->PawnVector[i] + 0xC00 + 0x40) = colorArgb;  // It's mandatory to use char* because void* doesn't allow poitner arithmetic
		*(DWORD *)((char *)cgame->PawnVector[i] + 0xC00 + 0x51) = 1;
	}
	
}


void Visuals::renderWithImgui ( ) {

	for (unsigned i = 0; i < cgame->PlayersVector.size ( );i++) {

		Vec3 feetPosition = (cgame->PlayersVector[i]->Pawn->vOldOrigin);

		Vec3 headPos = { feetPosition.x + 0.0f,feetPosition.y + 0.0f,feetPosition.z + 65.0f };

		Vec2 feet, head;

		if (feetPosition.WorldToScreen (feet, cgame->ViewMatrix) && headPos.WorldToScreen (head, cgame->ViewMatrix)) {
			float height = (feet.y - head.y) * 1.5f;
			float width = height / 1.5f;

			char playername[128] = {};

			std::memcpy (playername, &cgame->PlayersVector[i]->Controller->m_iszPlayerName, sizeof (playername));


			// Add health bar

			ImGui::GetBackgroundDrawList ( )->AddRect ({ (feet.x - width),head.y }, { (feet.x - width / 2) + width,head.y + height }, ImColor (255, 255, 255));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y }, ImColor (0, 255, 0), std::to_string (cgame->PlayersVector[i]->Pawn->pawnHealth).c_str ( ));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y - 20 }, ImColor (255, 200, 0), std::to_string (cgame->PlayersVector[i]->Pawn->m_armor).c_str ( ));
			ImGui::GetBackgroundDrawList ( )->AddText ({ feet.x - width, feet.y - 40 }, ImColor (0, 150, 255), playername);

			//ImGui::GetBackgroundDrawList()->AddText({ feet.x, feet.y }, ImColor(0, 255, 0), playername);
		}

	}
}


void Visuals::NoSmoke ( ) {
	for (unsigned int i = 0; i < cgame->SmokeGrenadeVector.size ( );i++) {
		cgame->SmokeGrenadeVector[i]->bDidSmokeEffect = true;
		//cgame->SmokeGrenadeVector[i]->smokeColor = *(Vec3*)(new Vec3(globals::smokeColorRGB[0], globals::smokeColorRGB[1], globals::smokeColorRGB[2]));
	}
}

void Visuals::changeSmokeColor (float smokeColor[3])
{
	for (unsigned int i = 0; i < cgame->SmokeGrenadeVector.size ( );i++) {
		cgame->SmokeGrenadeVector[i]->smokeColor = *(Vec3*)(new Vec3 (smokeColor[0], smokeColor[1], smokeColor[2]));
	}

}
