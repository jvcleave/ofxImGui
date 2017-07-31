#pragma once

#include "ofConstants.h"
#if defined(TARGET_OPENGLES) && (!defined (OF_TARGET_API_VULKAN) )

#include "BaseEngine.h"

#include "ofEvents.h"
#include "ofShader.h"
#include "imgui.h"

namespace ofxImGui
{
	class EngineOpenGLES 
		: public BaseEngine
	{
	public:
		~EngineOpenGLES()
		{
			exit();
		}

		// BaseEngine required
		void setup() override;
		void exit() override;
		bool createDeviceObjects() override;
		void invalidateDeviceObjects() override;

		void onKeyReleased(ofKeyEventArgs& event) override;

		// Custom 
		static void rendererDrawLists(ImDrawData * draw_data);

		static ofShader g_Shader;
	};
}

#endif