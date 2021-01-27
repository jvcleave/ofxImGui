#pragma once

#include "ofConstants.h"
#if defined(TARGET_OPENGLES) && !defined (OF_TARGET_API_VULKAN) && (!defined(TARGET_RASPBERRY_PI) || !defined(TARGET_GLFW_WINDOW))

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
		virtual void setup(bool autoDraw) override;
		virtual void exit() override;
		
		virtual void render() override;
		virtual void newFrame() override{};
		virtual void endFrame() override{};
		
		virtual bool updateFontsTexture() override{ return false; };
		
#if defined(OFXIMGUI_ENABLE_OF_BINDINGS)
		bool createDeviceObjects() override;
		void invalidateDeviceObjects() override;
		
		void onKeyReleased(ofKeyEventArgs& event) override;

		// Custom 
		static void rendererDrawData(ImDrawData * draw_data);
#endif

		static ofShader g_Shader;
	};
}

#endif
