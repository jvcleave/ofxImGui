#include "MyTheme.h"


ofColor getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255);    
}

void MyTheme::setup()
{
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
    
    
    style->Colors[ImGuiCol_Text] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(getRandomColor(), 0.58f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(getRandomColor(), 0.70f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(getRandomColor(), 0.58f);
    style->Colors[ImGuiCol_Border] = ImVec4(getRandomColor(), 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(getRandomColor(), 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(getRandomColor(), 0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(getRandomColor(), 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(getRandomColor(), 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(getRandomColor(), 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(getRandomColor(), 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(getRandomColor(), 0.80f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(getRandomColor(), 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(getRandomColor(), 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(getRandomColor(), 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(getRandomColor(), 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(getRandomColor(), 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(getRandomColor(), 0.32f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(getRandomColor(), 0.78f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(getRandomColor(), 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(getRandomColor(), 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(getRandomColor(), 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(getRandomColor(), 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(getRandomColor(), 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(getRandomColor(), 0.43f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(getRandomColor(), 0.92f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(getRandomColor(), 0.73f);
    
}
