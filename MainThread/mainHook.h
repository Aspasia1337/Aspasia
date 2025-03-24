
#include "font.h"

#include "../includes/includes.h"

#include <iostream>
#include <chrono>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
ID3D11RenderTargetView *mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	style.WindowMinSize = ImVec2 (160, 20);
	style.FramePadding = ImVec2 (6, 4);
	style.ItemSpacing = ImVec2 (8, 4);
	style.ItemInnerSpacing = ImVec2 (6, 4);
	style.Alpha = 0.95f;
	style.WindowRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.IndentSpacing = 10.0f;
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 12.0f;
	style.GrabRounding = 12.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 12.0f;

	style.Colors[ImGuiCol_Text] = ImVec4 (1.00f, 0.96f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4 (0.55f, 0.45f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4 (0.10f, 0.03f, 0.10f, 0.97f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4 (0.12f, 0.05f, 0.12f, 0.80f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4 (0.15f, 0.07f, 0.18f, 0.95f);
	style.Colors[ImGuiCol_Border] = ImVec4 (0.60f, 0.30f, 0.70f, 0.45f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4 (0.25f, 0.10f, 0.25f, 0.75f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 (0.45f, 0.20f, 0.50f, 0.90f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4 (0.60f, 0.25f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4 (0.18f, 0.07f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4 (0.42f, 0.18f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4 (0.10f, 0.05f, 0.15f, 0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4 (0.15f, 0.06f, 0.17f, 0.90f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4 (0.10f, 0.03f, 0.10f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4 (0.40f, 0.20f, 0.55f, 0.90f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 (0.50f, 0.25f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4 (0.65f, 0.30f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4 (0.85f, 0.50f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4 (0.70f, 0.30f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4 (0.90f, 0.20f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4 (0.75f, 0.20f, 0.55f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4 (0.85f, 0.30f, 0.70f, 0.90f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4 (0.60f, 0.15f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4 (0.80f, 0.25f, 0.55f, 0.50f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4 (0.85f, 0.35f, 0.75f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4 (0.90f, 0.10f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4 (0.50f, 0.25f, 0.70f, 0.60f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4 (0.70f, 0.30f, 0.90f, 0.80f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4 (0.90f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4 (0.60f, 0.20f, 0.75f, 0.40f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4 (0.80f, 0.30f, 0.90f, 0.70f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4 (0.90f, 0.10f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Tab] = ImVec4 (0.35f, 0.15f, 0.50f, 0.90f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4 (0.60f, 0.25f, 0.80f, 0.95f);
	style.Colors[ImGuiCol_TabActive] = ImVec4 (0.50f, 0.20f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4 (0.10f, 0.05f, 0.10f, 0.60f);

	ImFontConfig AspasiaFont;

	AspasiaFont.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF((void *)NeverloseFont, sizeof(NeverloseFont), 16.0f, &AspasiaFont);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (Globals::showMenu)
	{
		ImGuiIO &io = ImGui::GetIO();

		if (io.WantCaptureKeyboard && (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_CHAR))
			return 1;

		if (io.WantCaptureMouse && (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP ||
									uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP ||
									uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEWHEEL))
			return 1;

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);

		if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			return 1;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void DrawFovCircle()
{
	ImVec2 screenSize = ImGui::GetIO().DisplaySize;

	ImVec2 screenCenter = ImVec2(screenSize.x / 2, screenSize.y / 2);

	float fovRadius = (Globals::aimbotFov / 90.0f) * (screenSize.x / 2.0f);

	ImGui::GetBackgroundDrawList()->AddCircle(
		screenCenter,
		fovRadius,
		IM_COL32(255, 0, 0, 255),
		64,
		2.0f);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D *pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		ImGui::GetIO().MouseDrawCursor = !ImGui::GetIO().MouseDrawCursor;
		Globals::showMenu = !Globals::showMenu;
		if (Globals::MouseEvent)
			iHooksManager->m_IsRelativeMouseMode.hIsRelativeMouseFunction(Globals::MouseEvent, Globals::LastMode);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	std::time_t now = std::time(nullptr);
	std::tm localTime;

	static float gradientOffset = 0.0f;

	if (localtime_s(&localTime, &now) == 0)
	{
		ImGui::SetNextWindowPos({15, 15});
		ImGui::SetNextWindowSize({295, 35});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);

		ImGui::Begin("Watermark", NULL,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
						 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
						 ImGuiWindowFlags_NoScrollbar);

		ImGui::Text("Welcome %s | %02d:%02d:%02d | %d ms", 
			iGameEntitySystem->GetPlayerController()->m_sSanitizedPlayerName, localTime.tm_hour, 
			localTime.tm_min, localTime.tm_sec, Globals::ping);



		ImGui::End();
		ImGui::PopStyleVar();

		ImDrawList *drawList = ImGui::GetForegroundDrawList();

		ImVec2 p = ImVec2(15, 47);
		float width = 295.0f;
		float height = 4.0f;
		float rounding = 6.0f;

		ImColor color1 = ImColor::HSV(fmod(0.08f + gradientOffset, 1.0f), 0.8f, 0.9f);
		ImColor color2 = ImColor::HSV(fmod(0.12f + gradientOffset, 1.0f), 0.9f, 1.0f);

		drawList->AddRectFilledMultiColor(
			ImVec2(p.x, p.y),
			ImVec2(p.x + width, p.y + height),
			color1, color2, color2, color1);

		gradientOffset += 0.002f;
		if (gradientOffset > 1.0f)
			gradientOffset = 0.0f;
	}


	static float spectatorsGradientOffset = 0.0f;

	static float featuresGradientOffset = 0.0f;

	ImGui::SetNextWindowPos(ImVec2(15, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.85f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);

	if (ImGui::Begin("Features", NULL,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
						 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
						 ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Features:");
		ImGui::Separator();

		if (Globals::Features.empty())
		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "No features available.");
		}
		else
		{
			for (const auto &feature : Globals::Features)
			{
				if (*feature.bFeature)
					ImGui::BulletText("%s", feature.FeatureName);
			}
		}

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::End();

		ImDrawList *drawList = ImGui::GetForegroundDrawList();
		float width = windowSize.x;
		float height = 5.0f;
		float rounding = 6.0f;

		ImColor color1 = ImColor::HSV(fmod(0.08f + featuresGradientOffset, 1.0f), 0.8f, 0.9f);
		ImColor color2 = ImColor::HSV(fmod(0.12f + featuresGradientOffset, 1.0f), 0.9f, 1.0f);

		drawList->AddRectFilledMultiColor(
			ImVec2(windowPos.x, windowPos.y + windowSize.y - height),
			ImVec2(windowPos.x + width, windowPos.y + windowSize.y),
			color1, color2, color2, color1);

		drawList->AddRectFilled(
			ImVec2(windowPos.x, windowPos.y + windowSize.y - height),
			ImVec2(windowPos.x + width, windowPos.y + windowSize.y),
			ImColor(0, 0, 0, 50),
			rounding);

		featuresGradientOffset += 0.002f;
		if (featuresGradientOffset > 1.0f)
			featuresGradientOffset = 0.0f;
	}

	ImGui::PopStyleVar();

	if (Globals::ShowSpectators)
	{
		ImGui::SetNextWindowPos(ImVec2(15, 450));
		ImGui::SetNextWindowBgAlpha(0.85f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);

		if (ImGui::Begin("Spectators", NULL,
						 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
							 ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Spectators:");
			ImGui::Separator();

			if (Globals::Spectators.empty())
			{
				ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "No spectators.");
			}
			else
			{
				for (const auto &name : Globals::Spectators)
				{
					ImGui::BulletText("%s", name.c_str());
				}
			}

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			ImGui::End();

			ImDrawList *drawList = ImGui::GetForegroundDrawList();
			float width = windowSize.x;
			float height = 5.0f;
			float rounding = 6.0f;

			ImColor color1 = ImColor::HSV(fmod(0.08f + spectatorsGradientOffset, 1.0f), 0.8f, 0.9f);
			ImColor color2 = ImColor::HSV(fmod(0.12f + spectatorsGradientOffset, 1.0f), 0.9f, 1.0f);

			drawList->AddRectFilledMultiColor(
				ImVec2(windowPos.x, windowPos.y + windowSize.y - height),
				ImVec2(windowPos.x + width, windowPos.y + windowSize.y),
				color1, color2, color2, color1);

			drawList->AddRectFilled(
				ImVec2(windowPos.x, windowPos.y + windowSize.y - height),
				ImVec2(windowPos.x + width, windowPos.y + windowSize.y),
				ImColor(0, 0, 0, 50),
				rounding);

			spectatorsGradientOffset += 0.002f;
			if (spectatorsGradientOffset > 1.0f)
				spectatorsGradientOffset = 0.0f;
		}

		ImGui::PopStyleVar();
	}

	ImGui::SetNextWindowSize(ImVec2(800, 800));
	if (Globals::showMenu)
	{
		ImGui::Begin ("Aspasia 1337 : dev", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginTabBar ("MainTabs"))
		{
			// VISUALS EXPERIMENTAL
			if (ImGui::BeginTabItem ("Visuals (Experimental)"))
			{
				ImGui::TextColored (ImVec4 (1, 0.5f, 0.2f, 1), "VAC Features");

				if (ImGui::CollapsingHeader ("Glow"))
				{
					ImGui::Checkbox ("Enable Glow", &Globals::glow);
					if (Globals::glow)
					{
						ImGui::InputInt ("Glow Type", &Globals::glowType);
						ImGui::ColorEdit4 ("Glow Color", Globals::chamsColor, ImGuiColorEditFlags_NoInputs);
					}
				}

				if (ImGui::CollapsingHeader ("Smoke"))
				{
					ImGui::Checkbox ("No Smoke", &Globals::noSmoke);
					ImGui::Checkbox ("Smoke Visuals", &Globals::smokeVisuals);
					if (Globals::smokeVisuals)
					{
						ImGui::ColorEdit3 ("Smoke Color", Globals::smokeColorRGB, ImGuiColorEditFlags_NoInputs);
					}
				}

				if (ImGui::CollapsingHeader ("Info Overlay"))
				{
					ImGui::Checkbox ("Show Info Boxes", &Globals::ShowInfo);
				}

				ImGui::EndTabItem ( );
			}

			// HOOKS & EXPLOITS
			if (ImGui::BeginTabItem ("Hooks & Exploits"))
			{
				if (ImGui::CollapsingHeader ("Render Hooks"))
				{
					ImGui::Checkbox ("Render Smoke Hook", &Globals::RenderSmokeHook);
					ImGui::Checkbox ("Render Flash Hook", &Globals::RenderFlashHook);
				}

				if (ImGui::CollapsingHeader ("Angle Hook"))
				{
					ImGui::Checkbox ("View Angles Hook", &Globals::SetViewAngles);
					if (Globals::SetViewAngles)
					{
						ImGui::DragFloat3 ("View Angles", reinterpret_cast<float *>(&Globals::viewAngles), 0.1f);
					}
				}

				if (ImGui::CollapsingHeader ("Aim Assist Settings"))
				{
					ImGui::DragFloat ("Smoothing", &Globals::smoothing, 0.1f, 0.01f, 100.f);
					ImGui::DragFloat ("Aimbot FOV", &Globals::aimbotFov, 0.1f, 0.1f, 180.f);
				}

				if (ImGui::CollapsingHeader ("Exploits & Visuals"))
				{
					ImGui::Checkbox ("Anti Aim", &Globals::anti_aim);
					ImGui::Checkbox ("Bhop", &Globals::bhop);
					ImGui::Checkbox ("Third Person", &Globals::thirdPerson);

					ImGui::DragFloat ("FOV 1", &Globals::fov1, 0.1f, 0.1f, 180.f);
					ImGui::DragFloat ("FOV 2", &Globals::fov2, 0.1f, 0.1f, 180.f);
					ImGui::DragFloat ("FOV 3", &Globals::fov3, 0.1f, 0.1f, 180.f);

					ImGui::DragFloat ("Light Intensity", &Globals::lightIntensity, 0.001f, 0.0f, 10.0f, "%.3f");
				}

				ImGui::EndTabItem ( );
			}

			// LEGIT BOT
			if (ImGui::BeginTabItem ("Legit Bot"))
			{
				ImGui::DragFloat ("Pitch Angle", &Globals::AAPitch, 15.f, -90.f, 90.f);
				ImGui::DragFloat ("Jaw Angle", &Globals::AAJaw, 15.f, -180.f, 180.f);
				ImGui::Checkbox ("Show Spectators", &Globals::ShowSpectators);

				if (ImGui::CollapsingHeader ("Sky & World Colors"))
				{
					ImGui::Checkbox ("Change Sky Color", &Globals::ChangeSkyColor);
					if (Globals::ChangeSkyColor)
					{
						ImGui::ColorEdit3 ("Sky Color", Globals::SkyTintColor, ImGuiColorEditFlags_NoInputs);
					}

					ImGui::Checkbox ("World Modulation", &Globals::worldModulation);
					if (Globals::worldModulation)
					{
						ImGui::ColorEdit3 ("World Color", Globals::worldModulationColor, ImGuiColorEditFlags_NoInputs);
					}

					ImGui::Checkbox ("Light Modulation", &Globals::lightModulation);
					if (Globals::lightModulation)
					{
						ImGui::ColorEdit3 ("Light Color", Globals::lightModulationColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_HDR);
					}
				}

				ImGui::Checkbox ("Health Bar", &Globals::HealthBar);
				ImGui::Checkbox ("Render Skeleton", &Globals::RenderSkeleton);
				ImGui::Checkbox ("Render Weapon", &Globals::PlayerInfo);
				ImGui::Checkbox ("Render Bone Id", &Globals::RenderPlayerBones);
				ImGui::Checkbox ("Remove Scope", &Globals::RemoveScope);

				ImGui::Checkbox ("CreateMove Hook", &Globals::CreateMoveHook);
				if (Globals::CreateMoveHook)
				{
					ImGui::Combo ("Target Bone", &Globals::BoneSelected, Globals::BoneNames, IM_ARRAYSIZE (Globals::BoneNames));
				}

				ImGui::EndTabItem ( );
			}

			// CHAMS
			if (ImGui::BeginTabItem ("Chams"))
			{
				ImGui::Checkbox ("Enable Chams", &Globals::Chams);
				if (Globals::Chams)
				{
					ImGui::Checkbox ("Player Chams", &Globals::PlayersChams);
					if (Globals::PlayersChams)
					{
						ImGui::ColorEdit3 ("Chams Color", Globals::ChamsColor, ImGuiColorEditFlags_NoInputs);
						if (ImGui::Checkbox ("Render Invisible Chams", &Globals::InvisibleChams)) {

						}
						if (Globals::InvisibleChams) {
							ImGui::ColorEdit3 ("Invisible Chams Color", Globals::InvisibleChamsColor, ImGuiColorEditFlags_NoInputs);
						}
						ImGui::Combo ("Player Material", &Globals::MaterialSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));
					}

					if (ImGui::Checkbox ("Render Only Enemy Players", &Globals::OnlyEnemyRender)) {

					}

					if (ImGui::CollapsingHeader ("Weapon Chams"))
					{
						ImGui::Checkbox ("Weapons", &Globals::WeaponChams);
						if (Globals::WeaponChams)
						{
							ImGui::ColorEdit3 ("Gun Color", Globals::ChamsColorGun, ImGuiColorEditFlags_NoInputs);
							ImGui::Combo ("Gun Material", &Globals::MaterialGunSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));
						}
					}

					if (ImGui::CollapsingHeader ("Arms Chams"))
					{
						ImGui::Checkbox ("Arms", &Globals::ArmsChams);
						if (Globals::ArmsChams)
						{
							ImGui::ColorEdit3 ("Arms Color", Globals::ArmsColor, ImGuiColorEditFlags_NoInputs);
							ImGui::Combo ("Arms Material", &Globals::MaterialArmSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));
						}
					}
				}

				ImGui::EndTabItem ( );
			}

			ImGui::EndTabBar ( );
		}

		ImGui::End ( );
	}

	if (Globals::noSmoke)
	{
	}

	if (Globals::smokeVisuals)
	{
	}

	if (Globals::CreateMoveHook)
	{
		DrawFovCircle();
	}

	if (Globals::HealthBar || Globals::PlayerInfo || Globals::RenderPlayerBones || Globals::RenderSkeleton)
	{
		for(auto & pawn : iGameEntitySystem->PawnMap)
			iVisual->m_OverlayRender.RenderAllPlayerVisuals ( pawn.second);
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (GetAsyncKeyState(VK_NUMPAD0) & 1)
	{
		kiero::shutdown();
	}

	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;

	iHooksManager->initHook();

	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void **)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}
