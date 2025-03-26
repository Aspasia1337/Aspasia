#include "custom.hpp"
using namespace ImGui;

void c_custom::prepared_popup( const char* id, const char* name, std::function<void( )> content ) {

    ImGui::SetNextWindowSize( { 230,230 } );

    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8,11 } );
    ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 4 );
    ImGui::PushStyleColor( ImGuiCol_PopupBg, ImVec4( ImColor(27, 28, 31) ) );
    PushStyleColor( ImGuiCol_Border, ImVec4( 1.f, 1.f, 1.f, 0.05f ) );

    if ( ImGui::BeginPopup( id ) ) {

        auto pos = ImGui::GetCurrentWindow( )->Pos, size = ImGui::GetCurrentWindow( )->Size;
        ImGui::GetCurrentWindow( )->DrawList->AddText( ImGui::GetIO( ).Fonts->Fonts[2], ImGui::GetIO( ).Fonts->Fonts[2]->FontSize - 2, ImVec2( pos.x + 15, pos.y + 10 ), ImColor( 1.f, 1.f, 1.f ), name );
        ImGui::GetCurrentWindow( )->DrawList->AddLine( ImVec2( pos.x + 10, pos.y + 35 ), ImVec2( pos.x + size.x - 10, pos.y + 35 ), ImColor( 1.f, 1.f, 1.f, 0.1f ), 2 );

        ImGui::SetCursorPos( { 10, 45 } );
        custom.group_box_alternative( id, ImVec2( size.x - 20, size.y - 55 ) );

        content( );

        custom.end_group_box_alternative( );

        ImGui::EndPopup();
    }

    ImGui::PopStyleColor( 2 );
    ImGui::PopStyleVar( 2 );
}

bool c_custom::settings_widget( const char* u_id ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( u_id );

    ImVec2 icon_size = GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( GetIO( ).Fonts->Fonts[3]->FontSize + 1, FLT_MAX, 0, "J" );

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb( pos + ImVec2( 0, 1 ), pos + icon_size + ImVec2( 0, 1 ) );

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( bb );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, { 0.f } } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( hovered ? 1.f : 0.5f ), 0.035f );

    draw->AddText( GetIO( ).Fonts->Fonts[3], GetIO( ).Fonts->Fonts[3]->FontSize + 1, bb.Min, GetColorU32( ImGuiCol_Text, value->second ), "J" );

    return pressed;
}

void c_custom::colored_rect( const char* str_id, const ImColor color, const ImVec2 size ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( str_id );

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb( pos, pos + size );

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( bb );

    RenderFrame( bb.Min, bb.Max, color, 0, 4 );

}

void c_custom::picker_widget( const char* str_id, const char* content, const ImVec2 size ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( str_id );

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb( pos, pos + size );

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( bb );
    //a
    RenderFrame( bb.Min, bb.Max, ImColor(18, 18, 22), 0, 4 );
    draw->AddRect( bb.Min, bb.Max, ImColor(38, 38, 44 ), 4 );
    draw->AddRectFilled( bb.Min, ImVec2( bb.Min.x + 50, bb.Max.y ), ImColor(38, 38, 44), 4, ImDrawFlags_RoundCornersLeft );

    RenderText( bb.Min + ImVec2( 50 / 2 - CalcTextSize( str_id, 0, 1 ).x / 2, bb.GetCenter( ).y - CalcTextSize( str_id, 0, 1 ).y / 2 - pos.y ), str_id );
    draw->AddText( bb.Min + ImVec2( 60, bb.GetCenter( ).y - CalcTextSize( content, 0, 1 ).y / 2 - pos.y ), ImColor( 1.f, 1.f, 1.f ), content );

}

bool c_custom::tab (const char *icon, const char *label, bool selected, float rounding, ImDrawFlags flags) {
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = GetCurrentWindow ( );

    ImGuiID id = window->GetID (icon);

    ImFont *icon_font = (GetIO ( ).Fonts->Fonts.Size > 4) ? GetIO ( ).Fonts->Fonts[4] : GetIO ( ).Fonts->Fonts[0];
    ImFont *text_font = (GetIO ( ).Fonts->Fonts.Size > 1) ? GetIO ( ).Fonts->Fonts[1] : GetIO ( ).Fonts->Fonts[0];

    ImVec2 label_size = CalcTextSize (label, 0, 1);
    ImVec2 icon_size = icon_font->CalcTextSizeA (icon_font->FontSize, FLT_MAX, 0, icon);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2 (label_size.x + 40, 40);
    ImRect bb (pos, pos + size);

    auto draw = window->DrawList;

    ItemAdd (bb, id);
    ItemSize (ImRect (bb.Min.x, bb.Min.y, bb.Max.x - 8, bb.Max.y));

    bool hovered, held;
    bool pressed = ButtonBehavior (bb, id, &hovered, &held);

    static std::unordered_map<ImGuiID, float> values;
    auto value = values.find (id);
    if (value == values.end ( )) {
        values.insert ({ id, 0.f });
        value = values.find (id);
    }

    value->second = ImLerp (value->second, (selected ? 1.f : 0.f), 0.045f);

    draw->AddRectFilled (bb.Min, bb.Max, to_vec4 (19, 20, 23, value->second * GetStyle ( ).Alpha), rounding, flags);

    draw->AddText (icon_font, icon_font->FontSize, ImVec2 (bb.Min.x + 10, bb.GetCenter ( ).y - icon_size.y / 2), GetColorU32 (ImGuiCol_Text2), icon);
    draw->AddText (icon_font, icon_font->FontSize, ImVec2 (bb.Min.x + 10, bb.GetCenter ( ).y - icon_size.y / 2), ImColor (226 / 255.f, 230 / 255.f, 233 / 255.f, 0.5f * value->second * GetStyle ( ).Alpha), icon);

    draw->AddText (text_font, text_font->FontSize, ImVec2 (bb.Min.x + 30, bb.GetCenter ( ).y - label_size.y / 2), GetColorU32 (ImGuiCol_Text2), label);
    draw->AddText (text_font, text_font->FontSize, ImVec2 (bb.Min.x + 30, bb.GetCenter ( ).y - label_size.y / 2), ImColor (226 / 255.f, 230 / 255.f, 233 / 255.f, 0.5f * value->second * GetStyle ( ).Alpha), label);

    return pressed;
}

struct sub_tab_structure {

    float animation, inversed;
};

bool c_custom::sub_tab( const char* label, bool selected ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb( pos, pos + label_size );

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( bb );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    static std::unordered_map < ImGuiID, sub_tab_structure > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, { 0.f, 0.f } } );
        value = values.find( id );
    }

    value->second.animation = ImLerp( value->second.animation, ( selected ? 1.f : 0.f ), 0.045f );
    value->second.inversed  = ImLerp( value->second.inversed,  ( selected ? 0.f : 1.f ), 0.045f );

    draw->AddText( bb.Min, GetColorU32( ImGuiCol_Text2, value->second.inversed ), label );
    draw->AddText( bb.Min, custom.get_accent_color( value->second.animation * GetStyle( ).Alpha ), label );

    return pressed;
}

void c_custom::group_box( const char* name, ImVec2 size_arg ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImVec2 pos = window->DC.CursorPos;

    auto name_size = GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, 0.f, name );

    BeginChild( std::string( name ).append( ".main" ).c_str( ), size_arg, false, ImGuiWindowFlags_NoScrollbar );

    GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, to_vec4( 25, 25, 31, custom.m_anim * GetStyle( ).Alpha ), 3 );
    GetWindowDrawList( )->AddRectFilled( pos, pos + ImVec2( size_arg.x, 26 ), to_vec4( 38, 38, 44, custom.m_anim * GetStyle( ).Alpha ), 3, ImDrawFlags_RoundCornersTop );

    GetWindowDrawList( )->AddRect( pos, pos + size_arg, to_vec4( 38, 38, 44, custom.m_anim * GetStyle( ).Alpha ), 3 );

    GetWindowDrawList( )->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, pos + ImVec2( 10, 6 ), GetColorU32( ImGuiCol_Text, custom.m_anim ), name );

    SetCursorPosY( 35 );
    BeginChild( name, { size_arg.x, size_arg.y - 35 } );
    SetCursorPosX( 12 );

    BeginGroup( );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 8 } );
    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim * GetStyle( ).Alpha );
}

void c_custom::end_group_box( ) {

    PopStyleVar( 2 );
    EndGroup( );
    EndChild( );
    EndChild( );
}

void c_custom::group_box_alternative( const char* name, ImVec2 size_arg, ImVec2 padding ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImVec2 pos = window->DC.CursorPos;

    auto name_size = GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, 0.f, name );

    BeginChild( std::string( name ).append( ".main" ).c_str( ), size_arg, false, ImGuiWindowFlags_NoScrollbar );
    
    GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, to_vec4(25, 25, 31, custom.m_anim * GetStyle( ).Alpha ), 3 );
    GetWindowDrawList( )->AddRect( pos, pos + size_arg, to_vec4(38, 38, 44, custom.m_anim * GetStyle( ).Alpha), 3 );

    SetCursorPosY( padding.y );
    BeginChild( name, { size_arg.x, size_arg.y - padding.y } );
    SetCursorPosX( padding.x );

    BeginGroup( );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 8 } );
    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim * GetStyle( ).Alpha );
}

void c_custom::end_group_box_alternative( ) {

    PopStyleVar( 2 );
    EndGroup( );
    EndChild( );
    EndChild( );
}

bool c_custom::checkbox( const char* label, bool* v, const char* hint, ImFont* font ) {

    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = font->CalcTextSizeA( font->FontSize, FLT_MAX, 0, label );
    const ImVec2 hint_size = GetIO( ).Fonts->Fonts[ 0 ]->CalcTextSizeA( GetIO( ).Fonts->Fonts[ 0 ]->FontSize - 1.f, FLT_MAX, 0, hint );

    const float square_sz = 14.f;
    const float ROUNDING = 1.f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect frame_bb( pos, pos + ImVec2( square_sz, square_sz ) );
    const ImRect total_bb( pos, pos + ImVec2( hint_size.x > label_size.x ? square_sz + hint_size.x + style.ItemInnerSpacing.x + 2 : square_sz + label_size.x + style.ItemInnerSpacing.x + 2, hint_size.x > 0 ? square_sz + 1 + hint_size.y : square_sz ) );
    ItemAdd( total_bb, id, &frame_bb );
    ItemSize( total_bb, style.FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( total_bb, id, &hovered, &held );
    if ( pressed )
    {
        *v = !( *v );
        MarkItemEdited( id );
    }

    static std::unordered_map< ImGuiID, float > values;
    auto value = values.find( id );

    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find(id);
    }

    value->second = ImLerp( value->second, ( *v ? 1.f : 0.f ), 0.07f );
    //38,38,44 to_vec4(38, 38, 44, style.Alpha)
    RenderFrame( frame_bb.Min, frame_bb.Max, to_vec4( 18,18,22, style.Alpha ), 0, ROUNDING );
    window->DrawList->AddRect( frame_bb.Min, frame_bb.Max, to_vec4(38, 38, 44, style.Alpha), ROUNDING );

    RenderFrame( frame_bb.Min, frame_bb.Max, custom.get_accent_color( value->second * style.Alpha ), 0, ROUNDING );

    window->DrawList->PushClipRect( frame_bb.GetCenter( ) - ImVec2( 8 / 2, 8 / 2 ), frame_bb.GetCenter( ) + ImVec2( 6 * value->second, 8 ) );
    RenderCheckMark( window->DrawList, frame_bb.GetCenter( ) - ImVec2( 8 / 2, 8 / 2 ), ImColor( 0.f, 0.f, 0.f, value->second * style.Alpha ), 8 );
    window->DrawList->PopClipRect( );

    ImVec2 label_pos = ImVec2( frame_bb.Max.x + style.ItemInnerSpacing.x + 2, frame_bb.GetCenter( ).y - label_size.y / 2 );
    if ( label_size.x > 0.f )
        window->DrawList->AddText( font, font->FontSize, label_pos, font == GetIO( ).Fonts->Fonts[1] ? to_vec4( 255, 255, 255, style.Alpha ) : to_vec4(226, 230, 233, style.Alpha ), label, FindRenderedTextEnd( label ) );

    if ( hint_size.x > 0.f )
        window->DrawList->AddText( GetIO( ).Fonts->Fonts[ 0 ], GetIO( ).Fonts->Fonts[ 0 ]->FontSize - 1.f, ImVec2( label_pos.x, label_pos.y + label_size.y + 1 ), GetColorU32( ImGuiCol_Text, 0.7f ), hint, FindRenderedTextEnd( hint ) );

    return pressed;
}

bool c_custom::button( const char* label, const char* icon, const ImVec2& size_arg, ImGuiButtonFlags flags ) {

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, 0, label );
    const ImVec2 icon_size = GetIO( ).Fonts->Fonts[4]->CalcTextSizeA( GetIO( ).Fonts->Fonts[4]->FontSize, FLT_MAX, 0, icon );

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, false, 5);
    window->DrawList->AddRect( bb.Min, bb.Max, to_vec4(38, 38, 44, style.Alpha), 5 );

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");

    if ( icon_size.x > 0 )
        window->DrawList->AddText( GetIO( ).Fonts->Fonts[4], GetIO( ).Fonts->Fonts[4]->FontSize, bb.GetCenter( ) - icon_size / 2 - ImVec2( ( label_size.x / 2 ) + 3, 0 ), GetColorU32( ImGuiCol_Text ), icon );

    window->DrawList->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, bb.GetCenter( ) - label_size / 2 + ( icon_size.x > 0 ? ImVec2( ( icon_size.x / 2 ) + 3, 0 ) : ImVec2( 0, 0 ) ), ImColor( 1.f, 1.f, 1.f, style.Alpha ), label );

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool c_custom::button_alternative( const char* icon, const ImVec2& size_arg ) {

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( icon );
    const ImVec2 icon_size = GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, 0, icon );

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, icon_size.x + style.FramePadding.x * 2.0f, icon_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    // Render
    RenderNavHighlight(bb, id);
    RenderFrame( bb.Min, bb.Max, to_vec4( 36, 46, 61, ( hovered && held ) ? 1.f : hovered ? 0.85f : 0.7f ), false, 2 );

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");

    window->DrawList->AddText( GetIO( ).Fonts->Fonts[3], GetIO( ).Fonts->Fonts[3]->FontSize, bb.GetCenter( ) - icon_size / 2, GetColorU32( ImGuiCol_Text ), icon );

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

void c_custom::list_box( const char* name, ImVec2 size_arg, std::function< void( ) > content, ImVec2 padding ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImVec2 pos = window->DC.CursorPos;
    ImDrawList* draw = window->DrawList;

    auto id = window->GetID( name );
    auto name_size = CalcTextSize( name, 0, 1 );

    const ImRect bb( pos, pos + size_arg );
    const ImRect total_bb( pos, bb.Max );
    ItemAdd( total_bb, id );
    ItemSize( total_bb, GetStyle( ).FramePadding.y );

    draw->AddRectFilled( bb.Min, bb.Max, to_vec4(25, 25, 31, custom.m_anim), 3 );
    draw->AddRectFilled( bb.Min, bb.Min + ImVec2( size_arg.x, 26 ), to_vec4(38, 38, 44, custom.m_anim), 3, ImDrawFlags_RoundCornersTop );
    
    draw->AddRect( bb.Min, bb.Max, to_vec4(38, 38, 44, custom.m_anim), 3 );

    draw->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, bb.Min + ImVec2( 10, 6 ), GetColorU32( ImGuiCol_Text, custom.m_anim ), name );

    SetCursorPos( ImVec2( GetCursorPos( ).x + padding.x, GetCursorPos( ).y - GetStyle( ).ItemSpacing.y - size_arg.y + padding.y + 26 ) );

    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim );

    BeginChild( std::string( name ).append( ".child" ).c_str( ), ImVec2( size_arg.x - padding.x * 2, size_arg.y - padding.y * 2 - 26 ) );

    content( );

    EndChild( );

    PopStyleVar( );

    window->DC.CursorPos = ImVec2( bb.Min.x, bb.Max.y + GetStyle( ).ItemSpacing.y );

}

st_lua c_custom::lua( const char* label, bool selected ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size( { GetWindowWidth( ), 30 } );
    ImRect bb( pos, pos + size );

    ImVec2 icon_size = GetIO ( ).Fonts->Fonts[4]->CalcTextSizeA (GetIO ( ).Fonts->Fonts[4]->FontSize, FLT_MAX, 0, "E");

    ImRect status_bb( ImVec2( bb.Max.x - 17 - icon_size.x / 2, bb.GetCenter( ).y - icon_size.y / 2 ), ImVec2( bb.Max.x - 17 + icon_size.x / 2, bb.GetCenter( ).y + icon_size.y / 2 ) );

    bool s_hovered = false, s_pressed = false;
    if ( IsMouseHoveringRect( status_bb.Min, status_bb.Max ) ) {

        s_hovered = true;

        if ( IsMouseClicked( ImGuiMouseButton_Left ) )
            s_pressed = true;

    }

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( ImRect( bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y - 4 ) );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    static std::unordered_map < ImGuiID, st_lua > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, { 0.f, false, false, false } } );
        value = values.find( id );
    }

    value->second.animation = ImLerp( value->second.animation, ( selected ? 1.f : 0.f ), 0.05f );

    if ( s_pressed )
        value->second.active = !value->second.active;

    RenderFrame( bb.Min, bb.Max, to_vec4( 45, 55, 70, value->second.animation ), 0 );
    RenderFrame( bb.Min, ImVec2( bb.Min.x + 1, bb.Max.y ), custom.get_accent_color( GetStyle( ).Alpha * value->second.animation ) );

    RenderText( ImVec2( bb.Min.x + 10, bb.GetCenter( ).y - label_size.y / 2 ), label );
    draw->AddText( GetIO( ).Fonts->Fonts[4], GetIO( ).Fonts->Fonts[4]->FontSize, status_bb.Min, value->second.active ? custom.get_accent_color( GetStyle( ).Alpha ) : to_vec4( 116, 129, 148, GetStyle( ).Alpha ), value->second.active ? "E" : "F" );

    return { value->second.animation, pressed, s_pressed, value->second.active };
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool c_custom::begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
    const float size = 95;

    const ImRect total_bb( window->DC.CursorPos, window->DC.CursorPos + ImVec2( size, 30 ) );

    ItemAdd( total_bb, id, &total_bb );
    ItemSize( total_bb, style.FramePadding.y );

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    static std::unordered_map< ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( popup_open ? 1.f : 0.f ), 0.038f );

    window->DrawList->AddRectFilled( total_bb.Min, total_bb.Max, to_vec4(18, 18, 22, style.Alpha), 3 );
    window->DrawList->AddRect( total_bb.Min, total_bb.Max, to_vec4(38, 38, 44, style.Alpha), 3 );

    window->DrawList->AddCircleFilled( ImVec2( total_bb.Min.x + 13, total_bb.GetCenter( ).y ), 3.5f, preview_value == "Enemy" ? to_vec4( 248, 113, 113, style.Alpha ) : to_vec4( 74, 222, 128, style.Alpha ) );

    PushStyleColor( ImGuiCol_Text, ImVec4( 1.f, 1.f, 1.f, 0.7f * style.Alpha ) );
    RenderTextClipped( total_bb.Min + ImVec2( 25, 7 ), total_bb.Max - ImVec2( 20, -7 + -CalcTextSize( preview_value, 0, 1 ).y ), preview_value, NULL, NULL, ImVec2( 0.f, 0.f ) );
    PopStyleColor( );

    RenderArrow( window->DrawList, ImVec2( total_bb.Max.x - 18, total_bb.Min.y + 9 ), GetColorU32( ImGuiCol_Text, 0.7f ), ImGuiDir_Down, 10 );

    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size
    float w = total_bb.GetWidth();
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items) * value->second));
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(total_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, total_bb, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos( pos + ImVec2( 0, 3 ) );
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 4 ) ); // Horizontally align ourselves with the framed text
    PushStyleVar( ImGuiStyleVar_PopupRounding, 3 );
    PushStyleVar( ImGuiStyleVar_PopupBorderSize, 1 ); //to_vec4(38, 38, 44, style.Alpha)
    PushStyleColor( ImGuiCol_Border, ImVec4(ImColor(38, 38, 44)));
    PushStyleColor( ImGuiCol_PopupBg, ImVec4( ImColor(18, 18, 22) ) );
    bool ret = Begin(name, NULL, window_flags | ImGuiWindowFlags_NoScrollbar);
    PopStyleVar(3);
    PopStyleColor(2);
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

// Getter for the old Combo() API: const char*[]
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}

// Old API, prefer using BeginCombo() nowadays if you can.
bool c_custom::combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!begincombo(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID(i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
        if (selectable(item_text, item_selected, ImGuiSelectableFlags_NoPadWithHalfSpacing, ImVec2( 0, 30 )))
        {
            value_changed = true;
            *current_item = i;
        }
        PopStyleVar( );
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

// Combo box helper allowing to pass an array of strings.
bool c_custom::combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool c_custom::combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;   
    }
    bool value_changed = combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

bool c_custom::selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
        const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns)
    {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns)
    {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add)
        return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global) // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns && window->DC.CurrentColumns)
        PushColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePushBackgroundChannel();

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if (flags & ImGuiSelectableFlags_NoSetKeyOwner)     { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
    if (flags & ImGuiSelectableFlags_SelectOnClick)     { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if (flags & ImGuiSelectableFlags_SelectOnRelease)   { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick)  { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if (flags & ImGuiSelectableFlags_AllowItemOverlap)  { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
        if (g.NavJustMovedToId == id)
            selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
            g.NavDisableHighlight = true;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & ImGuiSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

    if (span_all_columns && window->DC.CurrentColumns)
        PopColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePopBackgroundChannel();

    static std::unordered_map< ImGuiID, float > values;
    auto value = values.find( id );

    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find(id);
    }

    value->second = ImLerp( value->second, ( selected ? 1.f : hovered ? 0.5f : 0.f ), 0.05f );

    RenderFrame( bb.Min, bb.Max, to_vec4(38, 38, 44, value->second), 0, 5 );

    window->DrawList->AddCircleFilled( ImVec2( bb.Min.x + 13, bb.GetCenter( ).y ), 3.5f, label == "Enemy" ? to_vec4( 248, 113, 113, style.Alpha ) : to_vec4( 74, 222, 128, style.Alpha ) );

    if ( selected || hovered )
        PushFont( GetIO( ).Fonts->Fonts[1] );

    PushStyleColor( ImGuiCol_Text, ImVec4( 1.f, 1.f, 1.f, ( selected ? 1.f : 0.5f ) ) );
    RenderText( ImVec2( bb.Min.x + 25, bb.GetCenter( ).y - label_size.y / 2 ), label );
    PopStyleColor( );

    if ( selected || hovered )
        PopFont( );

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed; //-V1020
}

bool c_custom::selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

void c_custom::image( ImTextureID user_texture_id, const ImVec2& size, float rounding, ImDrawFlags flags, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col ) {

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (border_col.w > 0.0f)
        bb.Max += ImVec2(2, 2);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    if (border_col.w > 0.0f)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), rounding, flags);
        window->DrawList->AddImageRounded(user_texture_id, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col), rounding, flags);
    }
    else
    {
        window->DrawList->AddImageRounded(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col), rounding, flags);
    }
}
