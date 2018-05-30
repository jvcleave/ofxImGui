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
		void setup() override;
	private:
		void exit() override;
		bool createDeviceObjects() override;
		void invalidateDeviceObjects() override;
		void destroyFontTextures() override;
		bool createFontsTexture();

	public:
		void onKeyReleased(ofKeyEventArgs& event) override;
		void onKeyPressed(ofKeyEventArgs& event) override;
		void onMousePressed(ofMouseEventArgs& event) override;

		// Custom 
		static void programmableRenderDrawLists(ImDrawData * draw_data);
		static void fixedRenderDrawLists(ImDrawData * draw_data);

		static GLuint g_FontTexture;
		static unsigned int g_VaoHandle;
	};
}

#endif