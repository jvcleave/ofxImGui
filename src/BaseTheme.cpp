#include "BaseTheme.h"



BaseTheme::BaseTheme()
{
    
    
    col_main_text = ofColor::white;
    col_main_head = ofColor::blue;
    col_main_area = ofColor::gray;
    col_win_popup = ofColor::yellow;
    col_win_backg = ofColor::black;

    col_main_text = ofColor::fromHex(0xdbede2);
    col_main_head = ofColor::fromHex(0xd12d49);
    col_main_area = ofColor::fromHex(0x333844);
    col_win_popup = ofColor::fromHex(0x77c4d3);
    col_win_backg = ofColor::fromHex(0x21232b);

}


void BaseTheme::setup()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowMinSize            = ImVec2(160, 65);
    style->FramePadding             = ImVec2(4, 2);
    style->ItemSpacing              = ImVec2(6, 2);
    style->ItemInnerSpacing         = ImVec2(6, 4);
    style->Alpha                    = 1.0f;
    style->WindowFillAlphaDefault   = 1.0f;
    style->WindowRounding           = 0.0f;
    style->FrameRounding            = 0.0f;
    style->IndentSpacing            = 6.0f;
    style->ItemInnerSpacing         = ImVec2(2, 4);
    style->ColumnsMinSpacing        = 50.0f;
    style->GrabMinSize              = 14.0f;
    style->GrabRounding             = 0.0f;
    style->ScrollbarSize            = 12.0f;
    style->ScrollbarRounding        = 0.0f;
}


void BaseTheme::updateColors()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    
    
    style->Colors[ImGuiCol_Text]                  = convertColor(col_main_text, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = convertColor(col_main_text, 0.58f);
    style->Colors[ImGuiCol_WindowBg]              = convertColor(col_win_backg, 1.00f);
    style->Colors[ImGuiCol_ChildWindowBg]         = convertColor(col_main_area, 0.58f);
    style->Colors[ImGuiCol_Border]                = convertColor(col_win_backg, 0.00f);
    style->Colors[ImGuiCol_BorderShadow]          = convertColor(col_win_backg, 0.00f);
    style->Colors[ImGuiCol_FrameBg]               = convertColor(col_main_area, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]        = convertColor(col_main_head, 0.78f);
    style->Colors[ImGuiCol_FrameBgActive]         = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = convertColor(col_main_area, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = convertColor(col_main_area, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive]         = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]             = convertColor(col_main_area, 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg]           = convertColor(col_main_area, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = convertColor(col_win_popup, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = convertColor(col_main_head, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_ComboBg]               = convertColor(col_main_area, 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = convertColor(col_main_head, 0.80f);
    style->Colors[ImGuiCol_SliderGrab]            = convertColor(col_win_popup, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive]      = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_Button]                = convertColor(col_win_popup, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered]         = convertColor(col_main_head, 0.86f);
    style->Colors[ImGuiCol_ButtonActive]          = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_Header]                = convertColor(col_main_head, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered]         = convertColor(col_main_head, 0.86f);
    style->Colors[ImGuiCol_HeaderActive]          = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_Column]                = convertColor(col_win_popup, 0.32f);
    style->Colors[ImGuiCol_ColumnHovered]         = convertColor(col_main_head, 0.78f);
    style->Colors[ImGuiCol_ColumnActive]          = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = convertColor(col_win_popup, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = convertColor(col_main_head, 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive]      = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_CloseButton]           = convertColor(col_main_text, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered]    = convertColor(col_main_text, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive]     = convertColor(col_main_text, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = convertColor(col_main_text, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = convertColor(col_main_text, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = convertColor(col_main_head, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = convertColor(col_main_head, 0.43f);
    style->Colors[ImGuiCol_TooltipBg]             = convertColor(col_win_popup, 0.92f);
    style->Colors[ImGuiCol_ModalWindowDarkening]  = convertColor(col_main_area, 0.73f);
}

inline
ImVec4 BaseTheme::convertColor(ofColor& color, float alpha)
{
    return ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, alpha);
}

inline
ImVec4 BaseTheme::convertColor(ofColor& color)
{
    return ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

inline
ofColor BaseTheme::convertColor(ImVec4& vec4)
{
    return ofColor(vec4.x *255, vec4.y *255, vec4.z *255, vec4.w);
}

inline
ofColor BaseTheme::convertColor(float* f)
{
    return ofColor(f[0] *255, f[1] *255, f[2] *255);
}


bool BaseTheme::addColorEdit(string label, ofColor& color)
{
    bool didChange = false;
    float floats[3];
    floats[0] = color.r / 255.f;
    floats[1] = color.g / 255.f;
    floats[2] = color.b / 255.f;
    didChange = ImGui::ColorEdit3(label.c_str(), &floats[0]);
    color = convertColor(&floats[0]);
    return didChange;
}

void BaseTheme::themeColorsWindow(bool isOpen)
{
    if(isOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(421, 192), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Theme Colors", &isOpen);
        
        bool b1 = addColorEdit("Text",        col_main_text);
        bool b2 = addColorEdit("Headers",     col_main_head);
        bool b3 = addColorEdit("Areas",       col_main_area);
        bool b4 = addColorEdit("Popups",      col_win_popup);
        bool b5 = addColorEdit("Background",  col_win_backg);
        
        if(b1 || b2 || b3 || b4 || b5)
        {
           updateColors();
        }
        

        ImGui::End();
    }
}