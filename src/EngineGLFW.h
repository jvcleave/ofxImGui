#pragma once

// Exclude Vulkan
#if !defined(OF_TARGET_API_VULKAN)

#include "ofConstants.h"

// This include is also used in the imgui glfw example, I hope it breaks nothing....
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "GLFW/glfw3.h"

#include "BaseEngine.h"

#include "ofEvents.h"
//#include "imgui.h"

// Draft implementation if enabled. Doesn't work in multi-contexts, but might allow intercepting gui-input later.
//#define INTERCEPT_GLFW_CALLBACKS 1 // Works better without the backend modification
#define INTERCEPT_GLFW_CALLBACKS 0 // Needs a modified backend for now (see imgui#3934)

#if INTERCEPT_GLFW_CALLBACKS == 1
#include <map>
class ImGuiContext; // fwd declare to prevent inclusion
struct GLFWwindow;

struct UniqueWindowData {
	ImGuiContext* imguiContext;
	ofAppBaseWindow* ofWindow;

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
		void setup(bool autoDraw) override;
		void exit() override;

        void newFrame() override;
        void endFrame() override;
        void render() override;

        bool updateFontsTexture() override;

		static GLuint g_FontTexture;

#if INTERCEPT_GLFW_CALLBACKS == 1
		// Below --> might be temporary, see https://github.com/ocornut/imgui/blob/9764adc7bb7a582601dd4dd1c34d4fa17ab5c8e8/backends/imgui_impl_glfw.cpp#L142-L145
		// We act as the imgui backend : unbinding OF events then calling them with the new functions again
		// Idea: Maybe not-so-tmp; this might also facilitate some nice hackery to (optionally) stop propagation to OF when ImGui is being used ? :D

		static void GlfwWindowFocusCallback(GLFWwindow* window, int focused);        // Since 1.84
		static void GlfwCursorEnterCallback(GLFWwindow* window, int entered);        // Since 1.84
		static void GlfwCursorPosCallback(GLFWwindow* window, double x, double y);   // Since 1.87
		static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GlfwCharCallback(GLFWwindow* window, unsigned int c);
		static void GlfwMonitorCallback(GLFWmonitor* monitor, int event);


		//void setCallbacks();
		//void removeCallbacks();
		static ImGuiContext* getImGuiContextOfWindow(const GLFWwindow*);
		static std::map<const GLFWwindow*, UniqueWindowData> glfwContexts; // for resolving bound instances juggled between events
#endif
	};
}

#endif
