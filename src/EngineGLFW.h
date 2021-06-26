#pragma once

// Exclude Vulkan
#if !defined(OF_TARGET_API_VULKAN)

#include "ofConstants.h"

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
