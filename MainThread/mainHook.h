
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

	style.WindowMinSize = ImVec2 (160, 20);
	style.FramePadding = ImVec2 (4, 2);
	style.ItemSpacing = ImVec2 (6, 2);
	style.ItemInnerSpacing = ImVec2 (6, 4);
	style.Alpha = 0.95f;
	style.WindowRounding = 4.0f;
	style.FrameRounding = 2.0f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2 (2, 4);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 14.0f;
	style.GrabRounding = 16.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;

	// Cambia el color del texto a verde neón
	style.Colors[ImGuiCol_Text] = ImVec4 (0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4 (0.0f, 0.6f, 0.0f, 0.5f);

	// Fondo oscuro verdoso
	style.Colors[ImGuiCol_WindowBg] = ImVec4 (0.1f, 0.2f, 0.1f, 0.8f);
	style.Colors[ImGuiCol_Border] = ImVec4 (0.0f, 0.5f, 0.0f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4 (0.0f, 0.0f, 0.0f, 0.0f);

	// Fondos de elementos oscuros
	style.Colors[ImGuiCol_FrameBg] = ImVec4 (0.05f, 0.15f, 0.05f, 0.8f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 (0.1f, 0.3f, 0.1f, 0.8f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4 (0.2f, 0.4f, 0.2f, 1.0f);

	// Títulos y barras de menú
	style.Colors[ImGuiCol_TitleBg] = ImVec4 (0.05f, 0.1f, 0.05f, 0.9f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4 (0.05f, 0.1f, 0.05f, 0.6f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4 (0.2f, 0.4f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4 (0.1f, 0.2f, 0.1f, 0.8f);

	// Scrollbars
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4 (0.05f, 0.15f, 0.05f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4 (0.2f, 0.4f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 (0.3f, 0.5f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4 (0.4f, 0.6f, 0.4f, 1.0f);

	// Botones
	style.Colors[ImGuiCol_Button] = ImVec4 (0.1f, 0.3f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4 (0.2f, 0.5f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4 (0.3f, 0.6f, 0.3f, 1.0f);

	// Headers
	style.Colors[ImGuiCol_Header] = ImVec4 (0.1f, 0.3f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4 (0.2f, 0.5f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4 (0.3f, 0.6f, 0.3f, 1.0f);

	// Fondos de modales
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4 (0.1f, 0.2f, 0.1f, 0.9f);



	ImFontConfig AspasiaFont;

	AspasiaFont.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF ((void *)NeverloseFont, sizeof (NeverloseFont), 16.0f, &AspasiaFont);

}

LRESULT __stdcall WndProc (const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler (hWnd, uMsg, wParam, lParam))
		return true; 

	if (globals::showMenu) {
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



	if (GetAsyncKeyState (VK_INSERT) & 1)
		globals::showMenu = !globals::showMenu;


	ImGui_ImplDX11_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );

	std::time_t now = std::time (nullptr);
	std::tm localTime;

	if (localtime_s (&localTime, &now) == 0) {
		ImGui::SetNextWindowPos ({ 15,15 });
		ImGui::SetNextWindowSize ({ 200,30 });

		ImGui::Begin ("Watermark", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		ImGui::Text ("Aspasia.win | %d:%d:%d", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);

		ImGui::End ( );
	}

	ImGui::SetNextWindowSize (ImVec2 (800, 800)); // Window Size
	if (globals::showMenu)
	{
		ImGui::Begin (("Aspasia 1337"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::Spacing ( );

			ImGui::SameLine ( );

			if (ImGui::Button (("Visuals"), ImVec2 (125, 40)))
			{
				globals::tab = 0;
			}

			ImGui::SameLine ( );
			if (ImGui::Button (("Function hooks!"), ImVec2 (125, 40)))
			{
				globals::tab = 1;


			}

			ImGui::Separator ( );

			ImGui::Dummy (ImVec2 (0.0f, 10.0f)); 

			if (globals::tab == 0) {

				if (ImGui::Checkbox ("Glow", &globals::glow)) {

					ImGui::SetTooltip ("Enable glow!");
				}

				if (globals::glow) {
					ImGui::InputInt ("Glow Type ", &globals::glowType);

					ImGui::ColorEdit4 ("Glow Visuals", globals::chamsColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions);
					
				}


				if (ImGui::Checkbox ("ShowInfo", &globals::ShowInfo)) {

					ImGui::SetTooltip ("Enable rectangles!");
				}

				if (ImGui::Checkbox ("Smoke Visuals", &globals::smokeVisuals)) {

				}
				if (globals::smokeVisuals) {
					if (ImGui::ColorEdit3 ("Smoke Color", &globals::smokeColorRGB[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions)) {
					
					}
				}


				if (ImGui::Checkbox ("No Smoke", &globals::noSmoke)) {
					
				}

			}


			if (globals::tab == 1) {
				if (ImGui::Checkbox ("Render Smoke Hook!", &globals::RenderSmokeHook)) {
				}
				if (ImGui::Checkbox ("Render Flash Hook!", &globals::RenderFlashHook)) {

				}
				if (ImGui::Checkbox ("CreateMove Hook!", &globals::CreateMoveHook)) {

				}
				if (ImGui::Checkbox ("View Angles Hook!", &globals::SetViewAngles)) {
				}

				if (globals::SetViewAngles) {
					ImGui::DragFloat3 ("Vector", reinterpret_cast<float *>(&globals::viewAngles), 0.1f, -360.0f, 360.0f, "%.2f");
				}


				if (ImGui::DragFloat ("smoothing", &globals::smoothing, 0.1f, 0.01f, 100.0f)) {
					if (globals::smoothing < 0.01f) globals::smoothing = 0.01f;
				}

				if (ImGui::DragFloat ("fov", &globals::aimbotFov, 0.1f, 0.1f, 180.0f)) {
					if (globals::aimbotFov < 0.1f) globals::aimbotFov = 0.1f;
				}
			}


		}

		ImGui::End ( );
	}




	if (globals::noSmoke) {
		iGameEntitySystem->getAllPlayers ( );

	}

	if (globals::smokeVisuals) {
		iGameEntitySystem->getAllPlayers ( );
		iVisuals->changeSmokeColor ( globals::smokeColorRGB);

	}

	if (globals::CreateMoveHook) {
		iGameEntitySystem->getEnemisByFov ( );
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
	iGameEntitySystem->init ( );

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

