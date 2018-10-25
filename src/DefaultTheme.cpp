#include "DefaultTheme.h"

namespace ofxImGui
{
    
    
    void DefaultTheme::setup()
    {
        ofColor col_main_text = ofColor::fromHex(0xdbede2);
        ofColor col_main_head = ofColor::fromHex(0xd12d49);
        ofColor col_main_area = ofColor::fromHex(0x333844);
        ofColor col_win_popup = ofColor::fromHex(0x77c4d3);
        ofColor col_win_backg = ofColor::fromHex(0x21232b);
        
        ImGuiStyle* style = &ImGui::GetStyle();
        
        style->WindowMinSize = ImVec2(160, 65);
        style->FramePadding = ImVec2(4, 2);
        style->ItemSpacing = ImVec2(6, 2);
        style->ItemInnerSpacing = ImVec2(6, 4);
        style->Alpha = 1.0f;
        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->IndentSpacing = 6.0f;
        style->ItemInnerSpacing = ImVec2(2, 4);
        style->ColumnsMinSpacing = 50.0f;
        style->GrabMinSize = 14.0f;
        style->GrabRounding = 0.0f;
        style->ScrollbarSize = 12.0f;
        style->ScrollbarRounding = 0.0f;
        
        style->Colors[ImGuiCol_Text] = ImVec4(col_main_text, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(col_main_text, 0.58f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(col_win_backg, 0.70f);
        style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_main_area, 0.58f);
        style->Colors[ImGuiCol_Border] = ImVec4(col_win_backg, 0.00f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(col_win_backg, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(col_main_area, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main_head, 0.78f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(col_main_area, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main_area, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(col_main_area, 0.47f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_main_area, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_win_popup, 0.21f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main_head, 0.78f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(col_main_head, 0.80f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(col_win_popup, 0.14f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(col_win_popup, 0.14f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main_head, 0.86f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(col_main_head, 0.76f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main_head, 0.86f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_Column] = ImVec4(col_win_popup, 0.32f);
        style->Colors[ImGuiCol_ColumnHovered] = ImVec4(col_main_head, 0.78f);
        style->Colors[ImGuiCol_ColumnActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(col_win_popup, 0.04f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main_head, 0.78f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(col_main_text, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(col_main_text, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main_head, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main_head, 0.43f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(col_win_backg, 0.92f);
        style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(col_main_area, 0.73f);
        
    }
}

