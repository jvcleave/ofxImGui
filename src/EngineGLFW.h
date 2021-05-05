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

		static GLuint g_FontTexture;
	};
}

#endif
