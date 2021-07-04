#pragma once

#include "ofConstants.h"

// Note : this implementation is broken, the file is kept for reference, if ported.

// Note: USE_GLFW_WINDOW is for ofxRPI4Window
#if defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)// && defined(USE_GLFW_WINDOW) //&& !defined(TARGET_RASPBERRY_PI_LEGACY)

#pragma GCC error "The OpenGLES implementation has not been updated yet ! (needs to be implemented)"

#ifdef TARGET_RASPBERRY_PI
#pragma GCC error "On oF 0.11 you should use a GLFW Window which supports GLES 1 2 and 3."
#pragma GCC error "On oF 0.10 and below, you should check out an older ofxImGui version. (below 1.80)"
#endif

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
