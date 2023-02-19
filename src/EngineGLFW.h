#pragma once

//#include "ofConstants.h"

// Warn Vulkan users
#if defined(OF_TARGET_API_VULKAN)
#pragma GCC error "Sorry, there's no tested support for Vulkan yet while it should be very easy to implement"
// See https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_vulkan.h or ImGui_ImplGlfw_InitForVulkan
//#include "backends/imgui_impl_vulkan.h"
#endif

// This include is also used in the imgui glfw example, I hope it breaks nothing....
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "BaseEngine.h"

// Enable a bit more complicated callback interface
// It fixes viewports in multiple contexts (one per ofAppWindow)
// If this stuff breaks, you can set this to 0 or add a define to your compilation settings.
// Todo: If enabled, ofxImGui will crash imgui on exit in Windows as the ofApp unregisters the callback @glfw then we still call it, crashing. OSX doesn't crash weirdly.
// We probably have to listen to window.events.notifyExit .
#ifndef INTERCEPT_GLFW_CALLBACKS
#define INTERCEPT_GLFW_CALLBACKS 1 // Works fine with the backend modification, otherwise breaks in multi-ofAppWindows
//#define INTERCEPT_GLFW_CALLBACKS 0 // Works natively too but not with multi-ofAppWindows
#endif

#if INTERCEPT_GLFW_CALLBACKS == 1

struct GLFWwindow;

#include "GLFW/glfw3.h"
#include "LinkedList.hpp"

struct GlfwCallbacks {
	// To store the original openFrameworks callbacks
	GLFWwindowfocusfun      originalCallbackWindowFocus;
	GLFWcursorenterfun      originalCallbackCursorEnter;
	GLFWcursorposfun        originalCallbackCursorPos;
	GLFWmousebuttonfun      originalCallbackMousebutton;
	GLFWscrollfun           originalCallbackScroll;
	GLFWkeyfun              originalCallbackKey;
	GLFWcharfun             originalCallbackChar;
	GLFWmonitorfun          originalCallbackMonitor;
};

#endif

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
		void setup(ofAppBaseWindow* _window, bool autoDraw) override;
		void exit() override;

        void newFrame() override;
        void render() override;

        bool updateFontsTexture() override;

		static GLuint g_FontTexture;

#if INTERCEPT_GLFW_CALLBACKS == 1
		// Below --> might be temporary, see https://github.com/ocornut/imgui/blob/9764adc7bb7a582601dd4dd1c34d4fa17ab5c8e8/backends/imgui_impl_glfw.cpp#L142-L145
		// We act as the imgui backend : unbinding OF events then calling them with the new functions again
		// Idea: Maybe not-so-tmp; this might also facilitate some nice hackery to (optionally) stop propagation to OF when ImGui is being used ? :D

		// These allow binding a static method and call the member functions below.
		// They simply route events to the engine instance of the corresponding window
		static void GlfwWindowFocusCallbackGlobal(GLFWwindow* window, int focused);        // Since 1.84
		static void GlfwCursorEnterCallbackGlobal(GLFWwindow* window, int entered);        // Since 1.84
		static void GlfwCursorPosCallbackGlobal(GLFWwindow* window, double x, double y);   // Since 1.87
		static void GlfwMouseButtonCallbackGlobal(GLFWwindow* window, int button, int action, int mods);
		static void GlfwScrollCallbackGlobal(GLFWwindow* window, double xoffset, double yoffset);
		static void GlfwKeyCallbackGlobal(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GlfwCharCallbackGlobal(GLFWwindow* window, unsigned int c);
		static void GlfwMonitorCallbackGlobal(GLFWmonitor* monitor, int event);

		// Non-static callbacks
		void GlfwWindowFocusCallback(GLFWwindow* window, int focused);        // Since 1.84
		void GlfwCursorEnterCallback(GLFWwindow* window, int entered);        // Since 1.84
		void GlfwCursorPosCallback(GLFWwindow* window, double x, double y);   // Since 1.87
		void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void GlfwCharCallback(GLFWwindow* window, unsigned int c);
		void GlfwMonitorCallback(GLFWmonitor* monitor, int event);

		static LinkedList<GLFWwindow, EngineGLFW> enginesMap;
		// Alternative: Use GLFW callback userdata to find back our object when the static callback is called
		// See https://github.com/ocornut/imgui/pull/3934#issuecomment-873213161
		// I didn't use this but it could give more flexibility.
		// Error : OF also sets GLFW user pointer. If we set it, OF breaks.

		// One storage for all original callbacks
		GlfwCallbacks originalOFCallbacks;
#endif
	};
}

