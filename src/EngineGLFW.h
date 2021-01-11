#pragma once

#include "ofConstants.h"
#include "GLFW/glfw3.h"
#if !defined(TARGET_OPENGLES) && (!defined (OF_TARGET_API_VULKAN) )

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
