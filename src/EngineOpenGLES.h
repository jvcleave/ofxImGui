#pragma once

#include "ofConstants.h"

// Note : this implementation is broken, the file is kept for reference, if ported.

#if defined(TARGET_OPENGLES) && !defined (OF_TARGET_API_VULKAN) && (!defined(TARGET_RASPBERRY_PI) || !defined(TARGET_GLFW_WINDOW))

#pragma GCC error "The OpenGLES implementation has not been updated yet ! (needs to be implemented)"

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

		static ofShader g_Shader;
	};
}

#endif
