#pragma once
#include "BaseTheme.h"



class ThemeCinder : public BaseTheme
{
public:
   
    void setup()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style = &ImGui::GetStyle();
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
        
        updateThemeColors();
    }
    
};