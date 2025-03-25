
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../includes/imgui/custom/custom.hpp"
#include "../includes/imgui/imgui_impl_dx11.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/blur/blur.hpp"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <tchar.h>
#include <chrono>
#include <string>
#include <time.h>

#include "../includes/includes.h"
#include "../bytes.hpp"

Present oPresent;
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device *pDevice = nullptr;
ID3D11DeviceContext *pContext = nullptr;
ID3D11RenderTargetView *mainRenderTargetView = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool menuOpen = false;
float menuAlpha = 0.f;

void InitImGui ( ) {
    ImGui::CreateContext ( );
    ImGuiIO &io = ImGui::GetIO ( );
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init (window);
    ImGui_ImplDX11_Init (pDevice, pContext);

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    config.OversampleH = 5;
    config.OversampleV = 5;
    config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x0400, 0x052F, 0xE000, 0xE226, 0 };
    config.GlyphRanges = ranges;

    io.Fonts->AddFontFromMemoryTTF (font_medium_binary, sizeof font_medium_binary, 15, &config, ranges);
    io.Fonts->AddFontFromMemoryTTF (font_bold_binary, sizeof font_bold_binary, 19, &config, ranges);
    io.Fonts->AddFontFromMemoryTTF (icons_binary, sizeof icons_binary, 13, &config, ranges);
}

void DrawMainUI ( ) {
    ImGui::PushStyleVar (ImGuiStyleVar_Alpha, menuAlpha);

    ImGui::SetNextWindowSize (ImVec2 (300 * menuAlpha, 100 * menuAlpha));
    ImGui::Begin ("Hello Window", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImVec2 pos = ImGui::GetWindowPos ( );
    ImDrawList *draw = ImGui::GetWindowDrawList ( );
    draw->AddRectFilled (pos, pos + ImVec2 (300, 100), ImColor (30, 30, 30, int (255 * menuAlpha)), 10.f);
    draw->AddText (pos + ImVec2 (20, 40), ImColor (255, 255, 255, int (255 * menuAlpha)), "Hello!");

    ImGui::End ( );
    ImGui::PopStyleVar ( );
}


void DrawFovCircle ( ) {
    ImVec2 center = ImGui::GetIO ( ).DisplaySize * 0.5f;
    float radius = (Globals::aimbotFov / 90.0f) * (ImGui::GetIO ( ).DisplaySize.x / 2.0f);
    ImGui::GetBackgroundDrawList ( )->AddCircle (center, radius, IM_COL32 (255, 0, 0, 255), 64, 2.0f);
}

LRESULT __stdcall WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler (hWnd, uMsg, wParam, lParam)) return true;

    if (Globals::showMenu) {
        ImGuiIO &io = ImGui::GetIO ( );
        if (io.WantCaptureMouse || io.WantCaptureKeyboard) return true;
    }

    return CallWindowProc (oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent (IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool init = false;

    if (!init) {
        if (SUCCEEDED (pSwapChain->GetDevice (__uuidof(ID3D11Device), (void **)&pDevice))) {
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
        else return oPresent (pSwapChain, SyncInterval, Flags);
    }

    if (GetAsyncKeyState (VK_INSERT) & 1) {
        ImGui::GetIO ( ).MouseDrawCursor = !ImGui::GetIO ( ).MouseDrawCursor;
        Globals::showMenu = !Globals::showMenu;
    }

    ImGui_ImplDX11_NewFrame ( );
    ImGui_ImplWin32_NewFrame ( );
    ImGui::NewFrame ( );

    menuAlpha = ImLerp (menuAlpha, Globals::showMenu ? 1.f : 0.f, 0.1f);

    if (Globals::showMenu || menuAlpha > 0.05f)
        DrawMainUI ( );

    if (Globals::CreateMoveHook)
        DrawFovCircle ( );

    ImGui::Render ( );
    pContext->OMSetRenderTargets (1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ( ));

    if (GetAsyncKeyState (VK_NUMPAD0) & 1)
        kiero::shutdown ( );

    return oPresent (pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread (LPVOID lpReserved) {
    iHooksManager->initHook ( );
    while (kiero::init (kiero::RenderType::D3D11) != kiero::Status::Success);
    kiero::bind (8, (void **)&oPresent, hkPresent);
    return TRUE;
}
