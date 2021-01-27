#pragma once

#include "ofEvents.h"
#include "ofAppBaseWindow.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace ofxImGui
{
	class BaseEngine
	{
	public:
		BaseEngine()
			: isSetup(false)
		{}

		virtual ~BaseEngine()
		{}

		virtual void setup(bool autoDraw) = 0;
		virtual void exit() = 0;

        virtual void newFrame() = 0;
        virtual void endFrame() = 0; // todo: remove ?
        virtual void render() = 0;

        virtual bool updateFontsTexture() = 0;

#if defined(OFXIMGUI_ENABLE_OF_BINDINGS)
        virtual bool createDeviceObjects() = 0;
        virtual void invalidateDeviceObjects() = 0;

		virtual void onMouseDragged(ofMouseEventArgs& event);
		virtual void onMousePressed(ofMouseEventArgs& event);
		virtual void onMouseReleased(ofMouseEventArgs& event);
		virtual void onMouseScrolled(ofMouseEventArgs& event);
		virtual void onKeyPressed(ofKeyEventArgs& event);
		virtual void onKeyReleased(ofKeyEventArgs& event) = 0;
		virtual void onWindowResized(ofResizeEventArgs& window);
#endif

		virtual GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

#if defined(OFXIMGUI_ENABLE_OF_BINDINGS)
		static const char* getClipboardString(void * userData);
		static void setClipboardString(void * userData, const char * text);

		static int g_ShaderHandle;
		static int g_VertHandle;
		static int g_FragHandle;

		static int g_UniformLocationTex;
		static int g_UniformLocationProjMtx;
		static int g_AttribLocationPosition;
		static int g_AttribLocationUV;
		static int g_AttribLocationColor;

		static unsigned int g_VboHandle;
		static unsigned int g_VaoHandle;
		static unsigned int g_ElementsHandle;

		static std::string g_ClipboardText;

		bool mousePressed[5] = { false };
#endif

	protected:
		bool isSetup;
	};
}

