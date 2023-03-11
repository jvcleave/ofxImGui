#pragma once

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
