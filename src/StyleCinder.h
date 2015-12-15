#pragma once
#include "ofMain.h"
#include "imgui.h"


class StyleCinder
{
public:
    StyleCinder()
    {
        style = NULL;
    }
    void setup()
    {
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
    
    void updateThemeColors()
    {
        if(!style)
        {
            style = &ImGui::GetStyle();
        }
        style->Colors[ImGuiCol_Text]                  = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled]          = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.58f);
        style->Colors[ImGuiCol_WindowBg]              = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_ChildWindowBg]         = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.58f);
        style->Colors[ImGuiCol_Border]                = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 0.00f);
        style->Colors[ImGuiCol_BorderShadow]          = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 0.00f);
        style->Colors[ImGuiCol_FrameBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
        style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_TitleBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.47f);
        style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.21f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
        style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_ComboBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_CheckMark]             = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.80f);
        style->Colors[ImGuiCol_SliderGrab]            = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.14f);
        style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_Button]                = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.14f);
        style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.86f);
        style->Colors[ImGuiCol_ButtonActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_Header]                = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.76f);
        style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.86f);
        style->Colors[ImGuiCol_HeaderActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_Column]                = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.32f);
        style->Colors[ImGuiCol_ColumnHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
        style->Colors[ImGuiCol_ColumnActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.04f);
        style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
        style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_CloseButton]           = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.16f);
        style->Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.39f);
        style->Colors[ImGuiCol_CloseButtonActive]     = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_PlotLines]             = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.43f);
        style->Colors[ImGuiCol_TooltipBg]             = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.92f);
        style->Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.73f);
    }
    
    void themeColorsWindow(bool * p_opened)
    {
        if(*p_opened)
        {
            ImGui::SetNextWindowSize(ImVec2(421, 192), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Theme Colors", p_opened);
            
            static float set_main_text[3] =
            {
                col_main_text.r / 255.f,
                col_main_text.g / 255.f,
                col_main_text.b / 255.f
            };
            bool changed_col_main_text = ImGui::ColorEdit3("Text",
                                                           &set_main_text[0]
                                                           );
            col_main_text = ofColor(
                                    set_main_text[0] * 255.f,
                                    set_main_text[1] * 255.f,
                                    set_main_text[2] * 255.f
                                    );
            
            static float set_main_head[3] =
            {
                col_main_head.r / 255.f,
                col_main_head.g / 255.f,
                col_main_head.b / 255.f
            };
            bool changed_col_main_head = ImGui::ColorEdit3("Headers",
                                                           &set_main_head[0]
                                                           );
            col_main_head = ofColor(
                                    set_main_head[0] * 255.f,
                                    set_main_head[1] * 255.f,
                                    set_main_head[2] * 255.f
                                    );
            
            static float set_main_area[3] =
            {
                col_main_area.r / 255.f,
                col_main_area.g / 255.f,
                col_main_area.b / 255.f
            };
            bool changed_col_main_area = ImGui::ColorEdit3("Areas",
                                                           &set_main_area[0]
                                                           );
            col_main_area = ofColor(
                                    set_main_area[0] * 255.f,
                                    set_main_area[1] * 255.f,
                                    set_main_area[2] * 255.f
                                    );
            
            static float set_win_popup[3] =
            {
                col_win_popup.r / 255.f,
                col_win_popup.g / 255.f,
                col_win_popup.b / 255.f
            };
            bool changed_col_win_popup = ImGui::ColorEdit3("Popups",
                                                           &set_win_popup[0]
                                                           );
            col_win_popup = ofColor(
                                    set_win_popup[0] * 255.f,
                                    set_win_popup[1] * 255.f,
                                    set_win_popup[2] * 255.f
                                    );
            
            static float set_win_backg[3] =
            {
                col_win_backg.r / 255.f,
                col_win_backg.g / 255.f,
                col_win_backg.b / 255.f
            };
            bool changed_col_win_backg = ImGui::ColorEdit3("Background",
                                                           &set_win_backg[0]
                                                           );
            col_win_backg = ofColor(
                                    set_win_backg[0] * 255.f,
                                    set_win_backg[1] * 255.f,
                                    set_win_backg[2] * 255.f
                                    );
            
            if(changed_col_main_text ||
               changed_col_main_head ||
               changed_col_main_area ||
               changed_col_win_popup ||
               changed_col_win_backg)
            {
                updateThemeColors();
            }
            
            ImGui::End();
        }
    }

    
    ImGuiStyle* style;
    
    ofColor col_main_text = ofColor::fromHex(0xdbede2);
    ofColor col_main_head = ofColor::fromHex(0xd12d49);
    ofColor col_main_area = ofColor::fromHex(0x333844);
    ofColor col_win_popup = ofColor::fromHex(0x77c4d3);
    ofColor col_win_backg = ofColor::fromHex(0x21232b);
};