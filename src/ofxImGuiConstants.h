// This file will set all macro defines for ofxImGui.
// You can modify it to quickly suit your needs
// But it's recommended to rather overr-de them using project-scope compiler defines.

//#pragma once

// Ensures they are always set (Only set if not forced by user).

// TODO:
// - rename OFXIMGUI_ENABLE_OF_BINDINGS to OFXIMGUI_FORCE_OF_BINDINGS for clarity

// Flags that you can set :
// #define OFXIMGUI_ENABLE_OF_BINDINGS --> force-use the simpler OF-based backend
// #define INTERCEPT_GLFW_CALLBACKS 0|1 --> disable imgui glfw backend modification to allow multiple context for using imgui with multiple ofAppBaseWindows

// Grab ofConstants
#include "ofConstants.h"

// Backend selection
#if defined (OFXIMGUI_ENABLE_OF_BINDINGS)
	#define OFXIMGUI_LOADED_BACKEND "OpenFrameworks"
	#define OFXIMGUI_BACKEND_OPENFRAMEWORKS
#elif defined (OF_TARGET_API_VULKAN)
	#define OFXIMGUI_LOADED_BACKEND "Vulkan"
	#define OFXIMGUI_BACKEND_VULKAN
	// Warn Vulkan users
	#if defined(OF_TARGET_API_VULKAN)
		#pragma GCC error "Sorry, there's no tested support for Vulkan yet while it should be very easy to implement"
		// See https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_vulkan.h or ImGui_ImplGlfw_InitForVulkan
	#endif
// Global condition for GLES platforms. Todo: Check if there are no false positives ?
#elif defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)
	#define OFXIMGUI_LOADED_BACKEND "GLES"
	#define OFXIMGUI_BACKEND_GLES
	#include "EngineOpenGLES.h"
// Platforms where GLFW is default don't define TARGET_GLFW_WINDOW, so we have to explictly add them
#elif defined(TARGET_GLFW_WINDOW) || (!defined(TARGET_OPENGLES) && (defined(TARGET_LINUX) || defined(TARGET_OSX)  || defined(TARGET_WIN32)))
	#include "EngineGLFW.h"
	#define OFXIMGUI_LOADED_BACKEND "GLFW"
	#define OFXIMGUI_BACKEND_GLFW
#else
	#pragma error "Sorry, could not configure ofxImGui on your openFrameworks configuration, nobody ever tried ofxImGui in your config. Feel free to as questions or submit a PR about support for your platform, we'd like to support all Platforms."
#endif

// Enable a bit more complicated callback interface
// if 1, it fixes viewports in multiple contexts (one per ofAppWindow)
// if 0, it uses the native imgui backend and multi-ofAppBaseWindows with pop-out windows (viewports) will fail.
#ifdef OFXIMGUI_BACKEND_GLFW
	#ifndef INTERCEPT_GLFW_CALLBACKS
		// You can toggle these lines to change the default value (enabled by default)
		#define INTERCEPT_GLFW_CALLBACKS 1
		//#define INTERCEPT_GLFW_CALLBACKS 0
	#endif
#else
	// Always off in non-GLFW contexts
	#define INTERCEPT_GLFW_CALLBACKS 0
#endif

// Uncomment to enable debugging
//#define OFXIMGUI_DEBUG


