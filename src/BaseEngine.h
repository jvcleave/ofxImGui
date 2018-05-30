#pragma once

#include "ofEvents.h"
#include "imgui.h"
#include "ofAppBaseWindow.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace ofxImGui
{
	class BaseEngine
	{
	public:
		BaseEngine()
			: isSetup(false)
		{
			getCount()++;
		}

		virtual ~BaseEngine()
		{
			getCount()--;
		}

		virtual void setup() = 0;
	private:
		virtual void exit() = 0;
		virtual bool createDeviceObjects() = 0;
		virtual void invalidateDeviceObjects() = 0;
		virtual void destroyFontTextures() = 0;
	public:
		virtual void onMouseDragged(ofMouseEventArgs& event);
		virtual void onMousePressed(ofMouseEventArgs& event);
		virtual void onMouseReleased(ofMouseEventArgs& event);
		virtual void onMouseScrolled(ofMouseEventArgs& event);
		virtual void onKeyPressed(ofKeyEventArgs& event);
		virtual void onKeyReleased(ofKeyEventArgs& event) = 0;
		virtual void onWindowResized(ofResizeEventArgs& window);

		ImGuiContext* ctx;
		ImGuiContext* getContext() { return ctx; };
		GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

		static const char* getClipboardString();
		static void setClipboardString(const char * text);

		static int g_ShaderHandle;
		static int g_VertHandle;
		static int g_FragHandle;

		static int g_UniformLocationTex;
		static int g_UniformLocationProjMtx;
		static int g_AttribLocationPosition;
		static int g_AttribLocationUV;
		static int g_AttribLocationColor;

		static unsigned int g_VboHandle;
		static unsigned int g_ElementsHandle;

		bool mousePressed[5] = { false };
		bool mouseReleased = true;

		// provides a count of the instances created for this class
		const int totalCount()
		{
//			cout << "current count " << getCount() << endl;
			return getCount();
		}

	protected:
		bool isSetup;

	private:
		static int & getCount()
		{
			static int theCount = 0;
			return theCount;
		}
	};
}

