#pragma once

#include "ofConstants.h"
#if !defined(TARGET_OPENGLES) && (!defined (OF_TARGET_API_VULKAN) )
#include "GLFW/glfw3.h"

#include "BaseEngine.h"

#include "ofEvents.h"
#include "imgui.h"

namespace ofxImGui
{
	class EngineGLFW 
		: public BaseEngine
	{
	public:
		~EngineGLFW()
		{
			exit();
		}

		// BaseEngine required
		void setup(bool autoDraw) override;
		void exit() override;
		bool createDeviceObjects() override;
		void invalidateDeviceObjects() override;

		void draw() override;

		bool createFontsTexture();

		void onKeyReleased(ofKeyEventArgs& event) override;
		void onKeyPressed(ofKeyEventArgs& event) override;
		void onMousePressed(ofMouseEventArgs& event) override;
		void onMouseReleased(ofMouseEventArgs& event) override;

		// Custom 
		static void programmableDrawData(ImDrawData * draw_data);
		static void fixedDrawData(ImDrawData * draw_data);

		static GLuint g_FontTexture;
	};
}

#endif
