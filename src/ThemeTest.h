#pragma once

#include "BaseTheme.h"

class ThemeTest 
	: public ofxImGui::BaseTheme
{
public:
	ThemeTest()
	{
		col_main_text = ofColor::gray;
		col_main_head = ofColor::orange;
		col_main_area = ofColor::green;
		col_win_popup = ofColor::yellow;
		col_win_backg = ofColor::blue;
	}
};