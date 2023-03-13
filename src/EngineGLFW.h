#pragma once

#include "ofxImGuiConstants.h"

#ifdef OFXIMGUI_BACKEND_GLFW

// This include is also used in the imgui glfw example, I hope it breaks nothing....
//#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <GLES2/gl2.h>
//#endif

#include "BaseEngine.h"


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
		EngineGLFW(){
			std::cout << "New BaseEngine " << this << std::endl;
		}
		~EngineGLFW()
		{
			exit();
		}

		// BaseEngine required
		void setup(ofAppBaseWindow* _window, ImGuiContext* _imguiContext, bool autoDraw) override;
		void exit() override;

        void newFrame() override;
        void render() override;

        bool updateFontsTexture() override;

		void onWindowExit(ofEventArgs& event);

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

		static LinkedList<GLFWwindow, EngineGLFW*> enginesMap;
		// Alternative: Use GLFW callback userdata to find back our object when the static callback is called
		// See https://github.com/ocornut/imgui/pull/3934#issuecomment-873213161
		// I didn't use this but it could give more flexibility.
		// Error : OF also sets the GLFW user pointer. If we set it, OF breaks.

		// One storage for all original callbacks
		GlfwCallbacks originalOFCallbacks;
#endif
	};
}
#endif
