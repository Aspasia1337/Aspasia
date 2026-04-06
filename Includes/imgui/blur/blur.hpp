#pragma once
#include<string>
#include<sstream>
#include<vector>
#include<math.h>

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "../imgui.h"
#include "../imgui_internal.h"
#include "../imgui_impl_dx11.h"
#include "../imgui_impl_win32.h"

namespace BlurData {
    inline IDirect3DDevice9 *device;
}

extern void DrawBackgroundBlur (ImDrawList *drawList);
