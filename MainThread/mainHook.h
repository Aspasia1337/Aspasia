
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Includes/imgui/custom/custom.hpp"
#include "../Includes/imgui/imgui_impl_dx11.h"
#include "../Includes/imgui/imgui_impl_win32.h"
#include "../Includes/imgui/blur/blur.hpp"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <tchar.h>
#include <chrono>
#include <string>
#include <time.h>

#include "../Includes/includes.h"
#include "../Includes/bytes.hpp"

Present oPresent;
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device *pDevice = nullptr;
ID3D11DeviceContext *pContext = nullptr;
ID3D11RenderTargetView *mainRenderTargetView = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

IDirect3DTexture9 *background = { nullptr };
IDirect3DTexture9 *weather = { nullptr };

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};


ImFont *fontMedium  = nullptr;
ImFont *fontSemi    = nullptr;
ImFont *fontBold    = nullptr;
ImFont *iconSmall   = nullptr;
ImFont *iconLarge   = nullptr;
ImFont *fontSemiBig = nullptr;


bool menuOpen = false;
float menuAlpha = 0.f;

const char *TempUserName = "??";

void RenderWatermark ( ) {
    extern ImFont *fontSemi;
    static float gradientOffset = 0.0f;
    std::time_t now = std::time (nullptr);
    std::tm localTime;

    if (localtime_s (&localTime, &now) == 0) {
        ImGui::SetNextWindowPos (ImVec2 (1530, 15), ImGuiCond_Always);
        ImGui::SetNextWindowSize (ImVec2 (295, 35));
        ImGui::SetNextWindowBgAlpha (0.85f);
        ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

        if (ImGui::Begin ("Watermark", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoScrollbar))
        {
            C_PlayerController *TempController = iGameEntitySystem->GetPlayerController ( );
            const char *TempUserName = "User";

            if (TempController)
                TempUserName = TempController->m_sSanitizedPlayerName;

            ImGui::PushFont (fontSemi);
            ImGui::Text ("Welcome %s | %02d:%02d:%02d | %d ms",
                TempUserName,
                localTime.tm_hour, localTime.tm_min, localTime.tm_sec,
                Globals::ping);
            ImGui::PopFont ( );

            ImGui::End ( );
        }

        ImGui::PopStyleVar ( );

        ImDrawList *drawList = ImGui::GetForegroundDrawList ( );
        ImVec2 p = ImVec2 (1530, 47);
        float width = 295.0f;
        float height = 4.0f;
        float rounding = 6.0f;

        ImColor color1 = ImColor::HSV (fmod (0.08f + gradientOffset, 1.0f), 0.8f, 0.9f);
        ImColor color2 = ImColor::HSV (fmod (0.12f + gradientOffset, 1.0f), 0.9f, 1.0f);

        drawList->AddRectFilledMultiColor (
            p,
            ImVec2 (p.x + width, p.y + height),
            color1, color2, color2, color1
        );

        drawList->AddRectFilled (
            ImVec2 (p.x, p.y),
            ImVec2 (p.x + width, p.y + height),
            ImColor (0, 0, 0, 50),
            rounding
        );

        gradientOffset += 0.002f;
        if (gradientOffset > 1.0f)
            gradientOffset = 0.0f;
    }
}


void RenderSpectatorList ( ) {
    static float spectatorsGradientOffset = 0.0f;

    if (!Globals::ShowSpectators) return;

    ImGui::SetNextWindowPos (ImVec2 (15, 450));
    ImGui::SetNextWindowBgAlpha (0.85f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

    if (ImGui::Begin ("Spectators", NULL,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoScrollbar)) {

        ImGui::TextColored (ImVec4 (1, 1, 1, 1), "Spectators:");
        ImGui::Separator ( );

        if (Globals::Spectators.empty ( )) {
            ImGui::TextColored (ImVec4 (0.8f, 0.8f, 0.8f, 1), "No spectators.");
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
        float height = 5.0f;
        ImColor color1 = ImColor::HSV (fmod (0.08f + spectatorsGradientOffset, 1.0f), 0.8f, 0.9f);
        ImColor color2 = ImColor::HSV (fmod (0.12f + spectatorsGradientOffset, 1.0f), 0.9f, 1.0f);

        drawList->AddRectFilledMultiColor (
            ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
            ImVec2 (windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            color1, color2, color2, color1);

        drawList->AddRectFilled (
            ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
            ImVec2 (windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            ImColor (0, 0, 0, 50), 6.0f);

        spectatorsGradientOffset = fmod (spectatorsGradientOffset + 0.002f, 1.0f);
    }

    ImGui::PopStyleVar ( );
}



void InitImGui ( ) {
    ImGui::CreateContext ( );
    ImGuiIO &io = ImGui::GetIO ( );
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init (window);
    ImGui_ImplDX11_Init (pDevice, pContext);


    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.FontDataOwnedByAtlas = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    io.Fonts->AddFontFromMemoryTTF (font_medium_binary, sizeof font_medium_binary, 15, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF (font_semi_binary, sizeof font_semi_binary, 15, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF (font_bold_binary, sizeof font_bold_binary, 19, &font_config, ranges);

    io.Fonts->AddFontFromMemoryTTF (icons_binary, sizeof icons_binary, 9, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF (icons_binary, sizeof icons_binary, 13, &font_config, ranges);

    io.Fonts->AddFontFromMemoryTTF (font_semi_binary, sizeof font_semi_binary, 17, &font_config, ranges);

    fontMedium  = io.Fonts->AddFontFromMemoryTTF (font_medium_binary, sizeof font_medium_binary, 15, &font_config, ranges);
    fontSemi    = io.Fonts->AddFontFromMemoryTTF (font_semi_binary, sizeof font_semi_binary, 15, &font_config, ranges);
    fontBold    = io.Fonts->AddFontFromMemoryTTF (font_bold_binary, sizeof font_bold_binary, 19, &font_config, ranges);
    iconSmall   = io.Fonts->AddFontFromMemoryTTF (icons_binary, sizeof icons_binary, 9, &font_config, ranges);
    iconLarge   = io.Fonts->AddFontFromMemoryTTF (icons_binary, sizeof icons_binary, 13, &font_config, ranges);
    fontSemiBig = io.Fonts->AddFontFromMemoryTTF (font_semi_binary, sizeof font_semi_binary, 17, &font_config, ranges);

    ImVec4 clear_color = ImVec4 (0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::GetBackgroundDrawList ( )->AddImage (background, ImVec2 (0, 0), ImGui::GetIO ( ).DisplaySize);
    BlurData::device = g_pd3dDevice;

    ImGuiStyle &style = ImGui::GetStyle ( );
    style.WindowRounding = 10.f;
    style.FrameRounding = 6.f;
    style.PopupRounding = 6.f;
    style.GrabRounding = 6.f;
    style.ScrollbarRounding = 6.f;

    style.WindowPadding = ImVec2 (10, 10);
    style.FramePadding = ImVec2 (8, 6);
    style.ItemSpacing = ImVec2 (8, 6);
    style.ItemInnerSpacing = ImVec2 (6, 5);


    ImVec4 *colors = style.Colors;

    colors[ImGuiCol_WindowBg] = ImVec4 (0.08f, 0.08f, 0.10f, 0.94f);
    colors[ImGuiCol_Header] = ImVec4 (0.4f, 0.3f, 0.6f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4 (0.5f, 0.4f, 0.7f, 0.75f);
    colors[ImGuiCol_HeaderActive] = ImVec4 (0.6f, 0.5f, 0.8f, 0.90f);

    colors[ImGuiCol_Button] = ImVec4 (0.35f, 0.3f, 0.6f, 0.6f);
    colors[ImGuiCol_ButtonHovered] = ImVec4 (0.45f, 0.4f, 0.7f, 0.8f);
    colors[ImGuiCol_ButtonActive] = ImVec4 (0.5f, 0.4f, 0.9f, 1.0f);

    colors[ImGuiCol_FrameBg] = ImVec4 (0.16f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4 (0.26f, 0.28f, 0.32f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4 (0.36f, 0.38f, 0.42f, 1.00f);

    colors[ImGuiCol_TitleBg] = ImVec4 (0.15f, 0.15f, 0.2f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4 (0.25f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4 (0.4f, 0.3f, 0.6f, 0.2f);

    colors[ImGuiCol_Text] = ImVec4 (0.95f, 0.95f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4 (0.6f, 0.6f, 0.8f, 1.0f);


    ImDrawList *draw = ImGui::GetBackgroundDrawList ( );
    draw->AddRectFilled (
        ImVec2 (0, 0),
        io.DisplaySize,
        ImColor (20, 15, 35, 200) 
    );


}

void RenderFeatureWindow ( ) {
    extern ImFont *fontSemi;
    static float featuresGradientOffset = 0.0f;

    ImGui::SetNextWindowPos (ImVec2 (15, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha (0.85f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 6.0f);

    if (ImGui::Begin ("Features", NULL,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::PushFont (fontSemi);
        ImGui::TextColored (ImColor (255, 255, 255), "Enabled Features:");
        ImGui::Separator ( );

        if (Globals::Features.empty ( )) {
            ImGui::TextColored (ImColor (200, 200, 200), "No features available.");
        }
        else {
            for (const auto &feature : Globals::Features) {
                if (*feature.bFeature)
                    ImGui::BulletText ("%s", feature.FeatureName);
            }
        }

        ImGui::PopFont ( );

        ImVec2 windowPos = ImGui::GetWindowPos ( );
        ImVec2 windowSize = ImGui::GetWindowSize ( );

        ImGui::End ( );

        ImDrawList *drawList = ImGui::GetForegroundDrawList ( );
        float height = 5.0f;
        float rounding = 6.0f;

        ImColor color1 = ImColor::HSV (fmod (0.08f + featuresGradientOffset, 1.0f), 0.8f, 0.9f);
        ImColor color2 = ImColor::HSV (fmod (0.12f + featuresGradientOffset, 1.0f), 0.9f, 1.0f);

        drawList->AddRectFilledMultiColor (
            ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
            ImVec2 (windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            color1, color2, color2, color1
        );

        drawList->AddRectFilled (
            ImVec2 (windowPos.x, windowPos.y + windowSize.y - height),
            ImVec2 (windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            ImColor (0, 0, 0, 50), rounding
        );

        // Animate
        featuresGradientOffset += 0.002f;
        if (featuresGradientOffset > 1.0f)
            featuresGradientOffset = 0.0f;
    }

    ImGui::PopStyleVar ( );
}




void DrawMainUI ( ) {
    static float alpha = 0.f;
    alpha = ImLerp (alpha, Globals::showMenu ? 1.f : 0.f, 0.15f);
    if (alpha < 0.05f) return;

    ImGuiIO &io = ImGui::GetIO ( );
    static ImVec2 winPos = ImVec2 (300, 200);
    ImVec2 winSize = ImVec2 (680, 500);

    extern ImFont *fontBold;
    extern ImFont *fontSemi;
    extern ImFont *fontSemiBig;

    ImGui::PushStyleVar (ImGuiStyleVar_Alpha, alpha);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 10.f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (10, 10));
    ImGui::PushStyleColor (ImGuiCol_WindowBg, ImVec4 (0.1f, 0.12f, 0.15f, alpha));

    ImGui::SetNextWindowSize (winSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos (winPos, ImGuiCond_FirstUseEver);
    static int currentTab = 0;

    if (ImGui::Begin ("AspasiaMainUI", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        if (ImGui::IsItemActive ( ) && ImGui::IsMouseDragging (ImGuiMouseButton_Left))
            winPos += io.MouseDelta;

        ImVec2 pos = ImGui::GetWindowPos ( );
        ImVec2 size = ImGui::GetWindowSize ( );
        ImDrawList *draw = ImGui::GetWindowDrawList ( );

        draw->AddRectFilled (pos, pos + ImVec2 (size.x, 40), ImColor (35, 39, 50, int (alpha * 255)), 10.0f, ImDrawFlags_RoundCornersTop);
        draw->AddRectFilled (pos + ImVec2 (0, 39), pos + ImVec2 (size.x, 40), ImColor (90, 140, 255, int (alpha * 255)));
        draw->AddText (
            fontBold,
            fontBold->FontSize,
            pos + ImVec2 (15, 12),
            ImColor (230, 220, 255, int (0.9f * alpha * 255)),
            "Aspasia Internal"
        );

        // Tabs
        ImGui::SetCursorPosY (50);
        const char *tabs[] = { "Visuals", "Hooks", "Legit", "Chams", "Anti Aim" };

        ImGui::SetCursorPosX (20);
        for (int i = 0; i < IM_ARRAYSIZE (tabs); ++i) {
            if (i > 0) ImGui::SameLine ( );

            ImGui::PushFont (fontSemi);
            if (i == currentTab) {
                ImGui::PushStyleColor (ImGuiCol_Button, ImColor (90, 140, 255).Value);
                if (ImGui::Button (tabs[i], ImVec2 (100, 30)))
                    currentTab = i;
                ImGui::PopStyleColor ( );
            }
            else {
                if (ImGui::Button (tabs[i], ImVec2 (100, 30)))
                    currentTab = i;
            }
            ImGui::PopFont ( );
        }

        // Main content
        ImGui::SetCursorPosY (90);
        ImGui::BeginChild ("MainContent", ImVec2 (-1, -1), true);
        ImGui::PushFont (fontSemiBig);

        ImGui::TextColored (ImColor (255, 255, 255), "%s Settings", tabs[currentTab]);
        ImGui::Separator ( );
        ImGui::PopFont ( );

        ImGui::PushFont (fontSemi);
        switch (currentTab) {
        case 0: // Visuals
            ImGui::Checkbox ("Health Bar", &Globals::HealthBar);
            ImGui::Checkbox ("Render Skeleton", &Globals::RenderSkeleton);
            ImGui::Checkbox ("Render Weapon", &Globals::PlayerInfo);
            ImGui::Checkbox ("Render Bone Id", &Globals::RenderPlayerBones);
            ImGui::Checkbox ("Remove Scope", &Globals::RemoveScope);

            ImGui::Spacing ( );
            ImGui::SeparatorText ("Sky & World Colors");

            ImGui::Checkbox ("Change Sky Color", &Globals::ChangeSkyColor);
            if (Globals::ChangeSkyColor) {
                ImGui::ColorEdit3 ("Sky Color", Globals::SkyTintColor, ImGuiColorEditFlags_NoInputs);
            }

            ImGui::Spacing ( );
            ImGui::Checkbox ("World Modulation", &Globals::worldModulation);
            if (Globals::worldModulation) {
                ImGui::ColorEdit3 ("World Color", Globals::worldModulationColor, ImGuiColorEditFlags_NoInputs);
            }

            ImGui::Spacing ( );
            ImGui::Checkbox ("Light Modulation", &Globals::lightModulation);
            if (Globals::lightModulation) {
                ImGui::ColorEdit3 ("Light Color", Globals::lightModulationColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_HDR);
                ImGui::DragFloat ("Light Intensity", &Globals::lightIntensity, 0.001f, 0.0f, 10.0f, "%.3f");
            }

            break;


        case 1: // Hooks
            ImGui::Checkbox ("Render Smoke Hook", &Globals::RenderSmokeHook);
            ImGui::Checkbox ("Render Flash Hook", &Globals::RenderFlashHook);
            ImGui::Checkbox ("View Angles Hook", &Globals::SetViewAngles);
            if (Globals::SetViewAngles)
                ImGui::DragFloat3 ("View Angles", (float *)&Globals::viewAngles, 0.1f);

            ImGui::Spacing ( );
            ImGui::SeparatorText ("Aimbot Settings");
            ImGui::DragFloat ("Smoothing", &Globals::smoothing, 0.1f, 0.01f, 100.f);
            ImGui::DragFloat ("Aimbot FOV", &Globals::aimbotFov, 0.1f, 0.1f, 180.f);

            ImGui::Spacing ( );
            ImGui::SeparatorText ("Misc");
            ImGui::Checkbox ("Anti Aim", &Globals::anti_aim);
            ImGui::Checkbox ("Bhop", &Globals::bhop);
            ImGui::Checkbox ("Third Person", &Globals::thirdPerson);
            break;

        case 2: // Legit
            ImGui::DragFloat ("Pitch Angle", &Globals::AAPitch, 15.f, -90.f, 90.f);
            ImGui::DragFloat ("Jaw Angle", &Globals::AAJaw, 15.f, -180.f, 180.f);
            ImGui::Checkbox ("Show Spectators", &Globals::ShowSpectators);

            ImGui::Spacing ( );
            ImGui::Checkbox ("Aimbot", &Globals::CreateMoveHook);
            if (Globals::CreateMoveHook)
                ImGui::Combo ("Target Bone", &Globals::BoneSelected, Globals::BoneNames, IM_ARRAYSIZE (Globals::BoneNames));
            break;

        case 3: // Chams
            ImGui::SeparatorText ("Player Chams");
            ImGui::Checkbox ("Player Chams", &Globals::PlayersChams);
            if (Globals::PlayersChams) {
                ImGui::ColorEdit3 ("Chams Color", Globals::ChamsColor, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox ("Invisible Chams", &Globals::InvisibleChams);
                if (Globals::InvisibleChams)
                    ImGui::ColorEdit3 ("Invisible Color", Globals::InvisibleChamsColor, ImGuiColorEditFlags_NoInputs);
                ImGui::Combo ("Material", &Globals::MaterialSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));

                ImGui::Checkbox ("PVS", &Globals::PVS);
            }

            ImGui::SeparatorText ("Weapon Chams");
            ImGui::Checkbox ("Weapon Chams", &Globals::WeaponChams);
            if (Globals::WeaponChams) {
                ImGui::ColorEdit3 ("Weapon Chams Color", Globals::ChamsColorGun, ImGuiColorEditFlags_NoInputs);
                ImGui::Combo ("Weapon Material", &Globals::MaterialGunSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));
            }

            ImGui::SeparatorText ("Arms Chams");
            ImGui::Checkbox ("Arms Chams", &Globals::ArmsChams);
            if (Globals::ArmsChams) {
                ImGui::ColorEdit3 ("Arms Chams Color", Globals::ArmsColor, ImGuiColorEditFlags_NoInputs);
                ImGui::Combo ("Arms Material", &Globals::MaterialArmSelected, Globals::MaterialNames, IM_ARRAYSIZE (Globals::MaterialNames));
            }
            break;

        case 4:
            ImGui::SeparatorText ("Anti-Aim");
            ImGui::Checkbox ("Rotational Anti-Aim", &Globals::AntiAim);
            if (Globals::AntiAim) {
                ImGui::SliderFloat ("Yaw Offset", &Globals::AAJaw, -180.0f, 180.0f, "%.1f");
                ImGui::SliderFloat ("Pitch Offset", &Globals::AAPitch, -89.0f, 89.0f, "%.1f");
            }

            ImGui::Checkbox ("Manual Anti Aim", &Globals::ManualAA);
            break;
        }


        ImGui::PopFont ( );
        ImGui::EndChild ( );

    }
    ImGui::End ( );
    ImGui::PopStyleColor ( );
    ImGui::PopStyleVar (3);
}

LRESULT __stdcall WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler (hWnd, uMsg, wParam, lParam)) return true;

    if (Globals::showMenu) {
        ImGuiIO &io = ImGui::GetIO ( );
        if (io.WantCaptureMouse || io.WantCaptureKeyboard) return true;
    }

    return CallWindowProc (oWndProc, hWnd, uMsg, wParam, lParam);
}

ID3D11BlendState *g_pBlendState = nullptr;

void CreateAlphaBlendState (ID3D11Device *device) {
    if (g_pBlendState) return;

    D3D11_BLEND_DESC desc = {};
    desc.RenderTarget[0].BlendEnable = TRUE;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    device->CreateBlendState (&desc, &g_pBlendState);
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
            CreateAlphaBlendState (pDevice);
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

    if (Globals::ManualAA) {
        if (GetAsyncKeyState (VK_UP) & 0x8000) {
            Globals::AAJaw = 0.0f;
        }
        if (GetAsyncKeyState (VK_DOWN) & 0x8000) {
            Globals::AAJaw = 180.0f;
        }
        if (GetAsyncKeyState (VK_LEFT) & 0x8000) {
            Globals::AAJaw = -90.0f;
        }
        if (GetAsyncKeyState (VK_RIGHT) & 0x8000) {
            Globals::AAJaw = 90.0f;
        }
    }


    ImGui_ImplDX11_NewFrame ( );
    ImGui_ImplWin32_NewFrame ( );
    ImGui::NewFrame ( );

    menuAlpha = ImLerp (menuAlpha, Globals::showMenu ? 1.f : 0.f, 0.1f);

    if (Globals::showMenu || menuAlpha > 0.05f) {
        DrawMainUI ( );
        RenderFeatureWindow ( );
    }

    if (Globals::ShowSpectators) {
        RenderSpectatorList ( );
    }

    // Watermark is currently shown only when the menu is open, mainly for testing and thesis screen captures.
    // For production use, consider rendering it independently of the menu visibility.
    RenderWatermark ( );

    if (Globals::HealthBar || Globals::PlayerInfo || Globals::RenderPlayerBones || Globals::RenderSkeleton){
        for (auto &pawn : iGameEntitySystem->PawnMap)
            iVisual->m_OverlayRender.RenderAllPlayerVisuals (pawn.second);
    }

    ImGui::Render ( );

    pContext->OMSetRenderTargets (1, &mainRenderTargetView, nullptr);
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
