#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include "../imgui.h"
#include "../imgui_internal.h"

#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#define to_vec4( r, g, b, a ) ImColor( r / 255.f, g / 255.f, b / 255.f, a )

struct st_lua {

    float animation;
    bool pressed_whole, pressed_icon, active;
};

class c_custom {

public:
    float m_anim = 0.f;

    float col_buf[4] = { 1.f, 1.f, 1.f, 1.f };

    int m_tab = 0;
    vector < const char* > tabs = { "Aimbot", "Visuals", "Misc" }, tabs_icons = { "A", "V", "M"};

    int m_aimbot_sub_tab = 0;
    vector < const char* > aimbot_sub_tabs = { "Ragebot", "Legitbot", "Miscellaneous" };

    void prepared_popup( const char* id, const char* name, std::function<void( )> content );
    bool settings_widget( const char* u_id );

    void colored_rect( const char* str_id, const ImColor color, const ImVec2 size );
    void picker_widget( const char* str_id, const char* content, const ImVec2 size );

    inline float get_label_sizes( const vector < const char* > vec ) {

        float result = {};
        for ( auto el : vec )
            result += ImGui::CalcTextSize( el, 0, 1 ).x;

        return result;
    }

    bool tab( const char* icon, const char* label, bool selected, float rounding = 0.f, ImDrawFlags flags = NULL );
    bool sub_tab( const char* label, bool selected );

    void group_box( const char* name, ImVec2 size_arg );
    void end_group_box( );

    void group_box_alternative( const char* name, ImVec2 size_arg, ImVec2 padding = ImVec2( 12, 12 ) );
    void end_group_box_alternative( );

    bool checkbox( const char* label, bool* v, const char* hint = "", ImFont* font = ImGui::GetIO( ).Fonts->Fonts[0] );

    bool button( const char* label, const char* icon, const ImVec2& size_arg = ImVec2( 0, 0 ), ImGuiButtonFlags flags = ImGuiButtonFlags_None );
    bool button_alternative( const char* icon, const ImVec2& size_arg = ImVec2( 0, 0 ) );

    void list_box( const char* name, ImVec2 size_arg, std::function< void( ) > content, ImVec2 padding = ImVec2( 8, 8 ) );

    st_lua lua( const char* label, bool selected );

    bool begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    bool selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

    void image( ImTextureID user_texture_id, const ImVec2& size, float rounding, ImDrawFlags flags, const ImVec2& uv0 = ImVec2( 0, 0 ), const ImVec2& uv1 = ImVec2( 1, 1 ), const ImVec4& tint_col = ImVec4( 1, 1, 1, 1 ), const ImVec4& border_col = ImVec4( 0, 0, 0, 0 ) );

    int m_accent_color[4] = { 112,171,255, 255 };
    ImColor get_accent_color( float a = 1.f ) {

        return to_vec4( m_accent_color[0], m_accent_color[1], m_accent_color[2], a );
    }

};

inline c_custom custom;
