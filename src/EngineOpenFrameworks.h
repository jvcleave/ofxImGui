#pragma once

#include "ofConstants.h"

//#include "ofxImGuiConstants.h" // To get the definition of INTERCEPT_GLFW_CALLBACKS
//#if defined(OFXIMGUI_BACKEND_GLFW)

// This include is also used in the imgui glfw example, I hope it breaks nothing....
#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <GLES2/gl2.h>
// includes from ofConstants.h / iOS
//#import <OpenGLES/ES1/gl.h>
//#import <OpenGLES/ES1/glext.h>

//#ifdef TARGET_LINUX_ARM
#ifdef TARGET_RASPBERRY_PI
#include "GLES/gl.h"
#include "GLES/glext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"
//#include "gles1CompatibilityHacks.h"
#else
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#endif

// Warn Vulkan users
#if defined(OF_TARGET_API_VULKAN)
#include "backends/imgui_impl_vulkan.h"
#elif defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)
// Todo: include GLSL stuff ?
#endif

#include "BaseEngine.h"

#include "ofEvents.h"
#include "imgui.h"

// This is the native openFrameworks ImGui custom PLATFORM backend.
// It uses the default imgui opengl 2+3 RENDERER backends.
// The platform backend is directly bound to openFrameworks so that it works in any project configuration.
// It's however recommended to use the native GLFW backend because:
// - Custom backends are harder to maintain between ImGui versions and have therefore less backend features implemented.
// - The native backends has more advanced feautures such as viewports support.

namespace ofxImGui
{
	class EngineOpenFrameworks
		: public BaseEngine
	{
	public:
		~EngineOpenFrameworks()
		{
			exit();
		}

		// BaseEngine required
		void setup(ofAppBaseWindow* _window, ImGuiContext* imguiContext, bool autoDraw) override;
		void exit() override;

        void newFrame() override;
        void render() override;

        bool updateFontsTexture() override;

        // Keyboard events
        void onKeyEvent(ofKeyEventArgs& event);
        void onCharInput(uint32_t& _char);

        // Mouse events
        void onMouseMoved(ofMouseEventArgs& event);
        void onMouseDragged(ofMouseEventArgs& event);
        void onMouseScrolled(ofMouseEventArgs& event);
        void onMouseButton(ofMouseEventArgs& event);

        // Window events
        void onWindowResized(ofResizeEventArgs& window);

        // todo: gamepad support ? io.AddKeyEvent()/io.AddKeyAnalogEvent()

        // Clipboard handling
        static const char* getClipboardString(void * userData);
        static void setClipboardString(void * userData, const char * text);
        static std::string g_ClipboardText; // Is this still useful ? Maybe ofWin->setClipboardString needs a reference that stays alive ?

        // Helpers
        static ImGuiKey oFKeyToImGuiKey(int key);
        static ImGuiKey keyCodeToImGuiKey(int keyCode);


	protected:
		void registerListeners();
		void unregisterListeners();
	};
}

