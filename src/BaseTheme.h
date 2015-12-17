#pragma once

#include "ofMain.h"
#include "imgui.h"


class BaseTheme
{
public:
    BaseTheme(){};
    virtual ~BaseTheme(){};
    virtual void setup()=0;
    static void themeColorsWindow(bool isOpen);

    static void updateThemeColors();
    
    static ofColor col_main_text;
    static ofColor col_main_head;
    static ofColor col_main_area;
    static ofColor col_win_popup;
    static ofColor col_win_backg;
};
