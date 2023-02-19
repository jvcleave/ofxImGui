#pragma once

#include "ofEvents.h"
#include "ofAppBaseWindow.h"

// Seems needed on windows
#include "imgui.h"

//#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace ofxImGui
{
	class BaseEngine
	{
			friend class EngineGLFW;
	public:
		BaseEngine()
			: isSetup(false)
		{}

		virtual ~BaseEngine()
		{}

		virtual void setup(ofAppBaseWindow* _window, bool autoDraw) = 0;
		virtual void exit() = 0;

		virtual void newFrame() = 0;
        virtual void render() = 0;

        virtual bool updateFontsTexture() = 0;

		// This is here to keep a compatibility layer with old codebases.
		// Not recommended to use, Upload with OF and use the GL_TEXTURE_2D's textureID directly.
		virtual GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

	protected:
		const char* getGLVersionFromOF() const;
		bool isSetup;

		// Context handling helpers for events
		bool setImGuiContext();
		void restoreImGuiContext();

		// Copy of associated ImGui context
		// For handling events, as we don't know which context is set when they are called
		ImGuiContext* imguiContext = nullptr;
		ImGuiContext* imguiContextPrev = nullptr;
	};
}

