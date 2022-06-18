#pragma once

// Exclude Vulkan
#if !defined(OF_TARGET_API_VULKAN)

#include "ofConstants.h"

// This include is also used in the imgui glfw example, I hope it breaks nothing....
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

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

		static GLuint g_FontTexture;
	};
}

#endif
