#pragma once

#include "ImGui.h"

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
