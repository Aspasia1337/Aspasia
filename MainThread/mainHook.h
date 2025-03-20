
#include "font.h"

#include "../visuals/Visuals.h"
#include "../includes/includes.h"

#include <iostream>
#include <chrono>

extern LRESULT ImGui_ImplWin32_WndProcHandler (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
ID3D11RenderTargetView *mainRenderTargetView;

void InitImGui ( )
{
	ImGui::CreateContext ( );
	ImGuiIO &io = ImGui::GetIO ( );
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init (window);
	ImGui_ImplDX11_Init (pDevice, pContext);

	ImGuiStyle &style = ImGui::GetStyle ( );
	ImVec4 *colors = style.Colors;

	style.WindowMinSize = ImVec2 (160, 20);
	style.FramePadding = ImVec2 (6, 4);
	style.ItemSpacing = ImVec2 (8, 4);
	style.ItemInnerSpacing = ImVec2 (6, 4);
	style.Alpha = 0.9f;
	style.WindowRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.IndentSpacing = 10.0f;
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 12.0f;
	style.GrabRounding = 12.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 12.0f;

	style.Colors[ImGuiCol_Text] = ImVec4 (1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4 (0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4 (0.02f, 0.01f, 0.02f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4 (0.08f, 0.08f, 0.08f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4 (0.71f, 0.60f, 0.91f, 0.33f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4 (0.10f, 0.07f, 0.12f, 0.89f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 (0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4 (0.29f, 0.28f, 0.34f, 0.94f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4 (0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4 (0.41f, 0.18f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4 (0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4 (0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4 (0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4 (0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 (0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4 (0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4 (0.60f, 0.20f, 0.87f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4 (0.65f, 0.24f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4 (0.88f, 0.06f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4 (0.86f, 0.18f, 0.61f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4 (0.76f, 0.21f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4 (0.40f, 0.10f, 0.52f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4 (0.97f, 0.21f, 0.49f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4 (0.87f, 0.37f, 0.65f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4 (0.78f, 0.10f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4 (0.25f, 0.18f, 0.86f, 0.50f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4 (0.42f, 0.13f, 0.69f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4 (0.55f, 0.04f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4 (0.78f, 0.50f, 0.87f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4 (0.54f, 0.14f, 0.92f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4 (0.51f, 0.04f, 0.86f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4 (0.23f, 0.13f, 0.40f, 0.86f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4 (0.45f, 0.23f, 0.86f, 0.80f);
	style.Colors[ImGuiCol_TabActive] = ImVec4 (0.30f, 0.17f, 0.76f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4 (0.80f, 0.80f, 0.80f, 0.35f);

	ImFontConfig AspasiaFont;

	AspasiaFont.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF ((void *)NeverloseFont, sizeof (NeverloseFont), 16.0f, &AspasiaFont);

}

LRESULT __stdcall WndProc (const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler (hWnd, uMsg, wParam, lParam))
		return true;

	if (Globals::showMenu) {
		ImGuiIO &io = ImGui::GetIO ( );

		if (io.WantCaptureKeyboard && (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_CHAR))
			return 1;

		if (io.WantCaptureMouse && (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP ||
			uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP ||
			uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEWHEEL))
			return 1;

		POINT cursorPos;
		GetCursorPos (&cursorPos);
		ScreenToClient (hWnd, &cursorPos);

		if (!ImGui::IsAnyItemHovered ( ) && !ImGui::IsWindowHovered (ImGuiHoveredFlags_AnyWindow)) {
			return 1;
		}
	}

	return CallWindowProc (oWndProc, hWnd, uMsg, wParam, lParam);
}

void DrawFovCircle ( ) {
	ImVec2 screenSize = ImGui::GetIO ( ).DisplaySize;

	ImVec2 screenCenter = ImVec2 (screenSize.x / 2, screenSize.y / 2);

	float fovRadius = (Globals::aimbotFov / 90.0f) * (screenSize.x / 2.0f);

	ImGui::GetBackgroundDrawList ( )->AddCircle (
		screenCenter,
		fovRadius,
		IM_COL32 (255, 0, 0, 255),
		64,
		2.0f
	);
}


bool init = false;
HRESULT __stdcall hkPresent (IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED (pSwapChain->GetDevice (__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pDevice->GetImmediateContext (&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc (&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D *pBackBuffer;
			pSwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
			pDevice->CreateRenderTargetView (pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release ( );
			oWndProc = (WNDPROC)SetWindowLongPtr (window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui ( );
			init = true;
		}

		else
			return oPresent (pSwapChain, SyncInterval, Flags);
	}



	if (GetAsyncKeyState (VK_INSERT) & 1) {
		ImGui::GetIO ( ).MouseDrawCursor = !ImGui::GetIO ( ).MouseDrawCursor;
		Globals::showMenu = !Globals::showMenu;
		if (Globals::MouseEvent)
			iHooksManager->m_IsRelativeMouseMode.hIsRelativeMouseFunction (Globals::MouseEvent, Globals::LastMode);
	}

	ImGui_ImplDX11_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );

	std::time_t now = std::time (nullptr);
	std::tm localTime;

	static float gradientOffset = 0.0f;

	if (localtime_s (&localTime, &now) == 0) {
		ImGui::SetNextWindowPos ({ 15, 15 });
		ImGui::SetNextWindowSize ({ 230, 35 });
		ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

		ImGui::Begin ("Watermark", NULL,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoScrollbar);

		ImGui::Text ("Aspasia.win | %02d:%02d:%02d | %d ms",
			localTime.tm_hour, localTime.tm_min, localTime.tm_sec, Globals::ping);

		ImGui::End ( );
		ImGui::PopStyleVar ( );

		ImDrawList *drawList = ImGui::GetForegroundDrawList ( );

		ImVec2 p = ImVec2 (15, 47);
		float width = 230.0f;
		float height = 4.0f;
		float rounding = 6.0f;

		ImColor color1 = ImColor::HSV (fmod (0.08f + gradientOffset, 1.0f), 0.8f, 0.9f); 
		ImColor color2 = ImColor::HSV (fmod (0.12f + gradientOffset, 1.0f), 0.9f, 1.0f); 

		drawList->AddRectFilledMultiColor (
			ImVec2 (p.x, p.y),
			ImVec2 (p.x + width, p.y + height),
			color1, color2, color2, color1 
		);

		gradientOffset += 0.002f;
		if (gradientOffset > 1.0f) gradientOffset = 0.0f;
	}


	static float spectatorsGradientOffset = 0.0f;



	static float featuresGradientOffset = 0.0f;

	ImGui::SetNextWindowPos (ImVec2 (15, 100), ImGuiCond_FirstUseEver); 
	ImGui::SetNextWindowBgAlpha (0.85f);
	ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

	if (ImGui::Begin ("Features", NULL,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::TextColored (ImVec4 (1.0f, 1.0f, 1.0f, 1.0f), "Features:"); 
		ImGui::Separator ( );

		if (Globals::Features.empty ( )) {
			ImGui::TextColored (ImVec4 (0.8f, 0.8f, 0.8f, 1.0f), "No features available.");
		}
		else {
			for (const auto &feature : Globals::Features) {
				if(*feature.bFeature)
				ImGui::BulletText ("%s", feature.FeatureName);
			}
		}

		ImVec2 windowPos = ImGui::GetWindowPos ( );
		ImVec2 windowSize = ImGui::GetWindowSize ( );

		ImGui::End ( );

		ImDrawList *drawList = ImGui::GetForegroundDrawList ( );
		float width = windowSize.x;
		float height = 5.0f;
		float rounding = 6.0f;

		ImColor color1 = ImColor::HSV (fmod (0.08f + featuresGradientOffset, 1.0f), 0.8f, 0.9f); 
		ImColor color2 = ImColor::HSV (fmod (0.12f + featuresGradientOffset, 1.0f), 0.9f, 1.0f); 

		drawList->AddRectFilledMultiColor (
			ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
			ImVec2 (windowPos.x + width, windowPos.y + windowSize.y),
			color1, color2, color2, color1
		);

		drawList->AddRectFilled (
			ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
			ImVec2 (windowPos.x + width, windowPos.y + windowSize.y),
			ImColor (0, 0, 0, 50),
			rounding
		);

		featuresGradientOffset += 0.002f;
		if (featuresGradientOffset > 1.0f) featuresGradientOffset = 0.0f;
	}

	ImGui::PopStyleVar ( );



	if (Globals::ShowSpectators) {
		ImGui::SetNextWindowPos (ImVec2 (15, 250));
		ImGui::SetNextWindowBgAlpha (0.85f);
		ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

		if (ImGui::Begin ("Spectators", NULL,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::TextColored (ImVec4 (1.0f, 1.0f, 1.0f, 1.0f), "Spectators:");
			ImGui::Separator ( );

			if (Globals::Spectators.empty ( )) {
				ImGui::TextColored (ImVec4 (0.8f, 0.8f, 0.8f, 1.0f), "No spectators.");
			}
			else {
				for (const auto &name : Globals::Spectators) {
					ImGui::BulletText ("%s", name.c_str ( ));
				}
			}

			ImVec2 windowPos = ImGui::GetWindowPos ( );
			ImVec2 windowSize = ImGui::GetWindowSize ( );

			ImGui::End ( );

			ImDrawList *drawList = ImGui::GetForegroundDrawList ( );
			float width = windowSize.x;  
			float height = 5.0f;
			float rounding = 6.0f; 

			ImColor color1 = ImColor::HSV (fmod (0.08f + spectatorsGradientOffset, 1.0f), 0.8f, 0.9f); 
			ImColor color2 = ImColor::HSV (fmod (0.12f + spectatorsGradientOffset, 1.0f), 0.9f, 1.0f); 

			drawList->AddRectFilledMultiColor (
				ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
				ImVec2 (windowPos.x + width, windowPos.y + windowSize.y),
				color1, color2, color2, color1
			);

			// 🔹 Bordes redondeados
			drawList->AddRectFilled (
				ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
				ImVec2 (windowPos.x + width, windowPos.y + windowSize.y),
				ImColor (0, 0, 0, 50), 
				rounding
			);

			// 🔹 Animación fluida del gradiente
			spectatorsGradientOffset += 0.002f;
			if (spectatorsGradientOffset > 1.0f) spectatorsGradientOffset = 0.0f;
		}

		ImGui::PopStyleVar ( );
	}

	ImGui::SetNextWindowSize (ImVec2 (800, 800)); 
	if (Globals::showMenu)
	{
		ImGui::Begin (("Aspasia 1337 : dev"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::Spacing ( );

			ImGui::SameLine ( );

			if (ImGui::Button (("Visuals"), ImVec2 (125, 40)))
			{
				Globals::tab = 0;
			}

			ImGui::SameLine ( );
			if (ImGui::Button (("Function hooks!"), ImVec2 (125, 40)))
			{
				Globals::tab = 1;
			}

			ImGui::SameLine ( );
			if (ImGui::Button (("Legit bot!"), ImVec2 (125, 40)))
			{
				Globals::tab = 2;
			}

			ImGui::Separator ( );

			ImGui::Dummy (ImVec2 (0.0f, 10.0f));

			if (Globals::tab == 0) {

				if (ImGui::Checkbox ("Glow", &Globals::glow)) {

					ImGui::SetTooltip ("Enable glow!");
				}

				if (Globals::glow) {
					ImGui::InputInt ("Glow Type ", &Globals::glowType);

					ImGui::ColorEdit4 ("Glow Visuals", Globals::chamsColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions);

				}


				if (ImGui::Checkbox ("ShowInfo", &Globals::ShowInfo)) {

					ImGui::SetTooltip ("Enable rectangles!");
				}

				if (ImGui::Checkbox ("Smoke Visuals", &Globals::smokeVisuals)) {

				}
				if (Globals::smokeVisuals) {
					if (ImGui::ColorEdit3 ("Smoke Color", &Globals::smokeColorRGB[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {

					}
				}


				if (ImGui::Checkbox ("No Smoke", &Globals::noSmoke)) {

				}

			}


			if (Globals::tab == 1) {
				if (ImGui::Checkbox ("Render Smoke Hook!", &Globals::RenderSmokeHook)) {
				}
				if (ImGui::Checkbox ("Render Flash Hook!", &Globals::RenderFlashHook)) {

				}

				if (ImGui::Checkbox ("View Angles Hook!", &Globals::SetViewAngles)) {
				}

				if (Globals::SetViewAngles) {
					ImGui::DragFloat3 ("Vector", reinterpret_cast<float *>(&Globals::viewAngles), 0.1f, -360.0f, 360.0f, "%.2f");
				}


				if (ImGui::DragFloat ("smoothing", &Globals::smoothing, 0.1f, 0.01f, 100.0f)) {
					if (Globals::smoothing < 0.01f) Globals::smoothing = 0.01f;
				}

				if (ImGui::DragFloat ("fov", &Globals::aimbotFov, 0.1f, 0.1f, 180.0f)) {
					if (Globals::aimbotFov < 0.1f) Globals::aimbotFov = 0.1f;
				}

				if (ImGui::Checkbox ("Anti Aim", &Globals::anti_aim)) {
				}

				if (ImGui::Checkbox ("bhop", &Globals::bhop)) {

				}


				if (ImGui::Checkbox ("AntiAim", &Globals::AntiAim)) {
					
				}
				if (ImGui::Checkbox ("Third Person", &Globals::thirdPerson)) {

				}

				if (ImGui::DragFloat ("fov1", &Globals::fov1, 0.1f, 0.1f, 180.0f)) {
				}
				if (ImGui::DragFloat ("fov2", &Globals::fov2, 0.1f, 0.1f, 180.0f)) {
				}
				if (ImGui::DragFloat ("fov3", &Globals::fov3, 0.1f, 0.1f, 180.0f)) {
				}

				if (ImGui::DragFloat ("intensity", &Globals::lightIntensity, 0.001f, 0.0f, 10.0f, "%.3f")) {
				}


			}


			if (Globals::tab == 2) {

				if (ImGui::DragFloat ("Pitch Angle", &Globals::AAPitch, 15.f, -90.0f, 90.0f, "%.2f")) {
				}
				if (ImGui::DragFloat ("Jaw Angle", &Globals::AAJaw, 15.f, -180.0f, 180.0f, "%.2f")) {
				}

				if (ImGui::Checkbox ("Show Spectators", &Globals::ShowSpectators)) {
				}

				if (ImGui::Checkbox ("Sky color", &Globals::ChangeSkyColor)) {

				}

				if (Globals::ChangeSkyColor) {
					if (ImGui::ColorEdit3 ("Sky Color##SkyTint", Globals::SkyTintColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {

					}
				}

				if (ImGui::Checkbox ("World Modulation", &Globals::worldModulation)) {

				}

				if (Globals::worldModulation) {
					if (ImGui::ColorEdit3 ("World Modulation##Color", Globals::worldModulationColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {

					}
				}

				if (ImGui::Checkbox ("Light Modulation", &Globals::lightModulation)) {

				}

				if (Globals::lightModulation) {
					// ImGuiColorEditFlags_HDR
					if (ImGui::ColorEdit3 ("Light Mod", Globals::lightModulationColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_HDR)) {

					}
				}

				if (ImGui::Checkbox ("Chams", &Globals::Chams)) {
					
				}

				if (ImGui::Checkbox ("HealtBar", &Globals::HealthBar)) {

				}

				if (ImGui::Checkbox ("Render Bones", &Globals::RenderPlayerBones)) {

				}

				if (Globals::Chams) {

					if (ImGui::Checkbox ("PlayerChams", &Globals::PlayersChams)) {

					}

					if (Globals::PlayersChams) {

						if (ImGui::ColorEdit3 ("Chams##Color", Globals::ChamsColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {

						}

						if (ImGui::Combo ("Chams Type Player", &Globals::MaterialSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames))) {

						}

					}

					if (ImGui::Checkbox ("Weapon Chams", &Globals::WeaponChams)) {

					}

					if (Globals::WeaponChams) {

						if (ImGui::ColorEdit3 ("Chams##ColorGun", Globals::ChamsColorGun, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {

						}

						if (ImGui::Combo ("Chams Type Gun", &Globals::MaterialGunSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames))) {

						}
					}



					if (ImGui::Checkbox ("LegitBot Hook!", &Globals::CreateMoveHook)) {

					}




				}


				
			if (ImGui::Checkbox ("CreateMove Hook!", &Globals::CreateMoveHook)) {


			}

			if (Globals::CreateMoveHook) {
				if (ImGui::Combo ("TARGET PLACE", &Globals::BoneSelected, Globals::BoneNames, IM_ARRAYSIZE (Globals::BoneNames))) {

				}

			}
				

			}
		}
		ImGui::End ( );
	}

	if (Globals::noSmoke) {
	}

	if (Globals::smokeVisuals) {
		iVisuals->changeSmokeColor (Globals::smokeColorRGB);
	}

	if (Globals::CreateMoveHook) {
		DrawFovCircle ( );
	}

	if (Globals::HealthBar) {
		for (auto &pawn : iGameEntitySystem->PawnMap) {
			iVisual->m_OverlayRender.RenderHealth (pawn.second);
		}
	}

	if (Globals::RenderPlayerBones) {
		for (auto &pawn : iGameEntitySystem->PawnMap) {
	//		iVisual->m_OverlayRender.RenderPlayerBones (pawn.second);
			iVisual->m_OverlayRender.RenderSkeleton (pawn.second);
		}
	}

	ImGui::Render ( );

	pContext->OMSetRenderTargets (1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ( ));


	if (GetAsyncKeyState (VK_NUMPAD0) & 1) {
		kiero::shutdown ( );
	}


	return oPresent (pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread (LPVOID lpReserved)
{
	bool init_hook = false;

	iHooksManager->initHook ( );


	do
	{
		if (kiero::init (kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind (8, (void **)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

