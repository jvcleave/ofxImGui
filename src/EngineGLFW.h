#pragma once

#include "ofConstants.h"

// Exclude Vulkan and RPI in LEGACY mode
#if (!defined(OF_TARGET_API_VULKAN) && (!defined(TARGET_RASPBERRY_PI) || (defined(TARGET_GLFW_WINDOW) && !defined(TARGET_RASPBERRY_PI_LEGACY)) ))
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

        void newFrame() override;
        void endFrame() override;
        void render() override;

        bool updateFontsTexture() override;

#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
        bool createFontsTexture();
        bool createDeviceObjects() override;
        void invalidateDeviceObjects() override;

		void onKeyReleased(ofKeyEventArgs& event) override;
		void onKeyPressed(ofKeyEventArgs& event) override;
		void onMousePressed(ofMouseEventArgs& event) override;
		void onMouseReleased(ofMouseEventArgs& event) override;

		// Custom 
		static void programmableDrawData(ImDrawData * draw_data);
		static void fixedDrawData(ImDrawData * draw_data);
#endif

		static GLuint g_FontTexture;
	};
}

#endif
