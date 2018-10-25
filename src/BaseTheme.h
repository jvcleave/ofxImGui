#pragma once

#include "ofColor.h"
#include "imgui.h"

namespace ofxImGui
{
	class BaseTheme
	{
	public:
        BaseTheme()
        {
            
        }
        virtual ~BaseTheme()
        {
            
        }
        
		virtual void setup()=0;
	};
}
