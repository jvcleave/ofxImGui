#pragma once

#include "ofColor.h"

namespace ofxImGui
{
	class BaseTheme
	{
	public:
		BaseTheme();

		virtual ~BaseTheme() {}

		virtual void setup();
		void themeColorsWindow(bool isOpen);

		void updateColors();

		ofColor col_main_text;
		ofColor col_main_head;
		ofColor col_main_area;
		ofColor col_win_popup;
		ofColor col_win_backg;

		bool addColorEdit(const std::string& label, ofColor& color);
		ofColor convertColor(float* f);
	};
}
