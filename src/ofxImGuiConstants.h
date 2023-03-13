// This file will set all macro defines for ofxImGui.
// You can modify it to quickly suit your needs
// But it's recommended to rather overr-de them using project-scope compiler defines.

//#pragma once

// Ensures they are always set (Only set if not forced by user).

// Flags that you can set :
// #define OFXIMGUI_FORCE_OF_BACKEND --> force-use the simpler OF-based backend
// #define INTERCEPT_GLFW_CALLBACKS 0|1 --> disable imgui glfw backend modification to allow multiple context for using imgui with multiple ofAppBaseWindows
// #define IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT 0|1 --> Disable modifications to imgui_impl_glfw, disabling support for using ofxImGui in a multi-windowed-ofApp with viewports enabled.

// TODO:
// - replace INTERCEPT_GLFW_CALLBACKS by OFXIMGUI_INTERCEPT_GLFW_CALLBACKS for clarity.
// - add OFXIMGUI_REPLACE_OF_GLFW_CALLBACKS to optionally listen to ofEvents rather than the GLFW callback ?

// Grab ofConstants
#include "ofConstants.h"

// Tmp
#ifndef OFXIMGUI_DEBUG
	// Uncomment to enable debugging
	//#define OFXIMGUI_DEBUG
#endif

// Helper for printing debug messages only when DEBUG is on
#ifdef OFXIMGUI_DEBUG
	#define PRAGMA_MESSAGE(x) _Pragma(#x)
	#define OFXIMGUI_COMPILER_MESSAGE(X) PRAGMA_MESSAGE(message X)
#else
	#define OFXIMGUI_COMPILER_MESSAGE(X)
#endif

// Platform backend selection
#if !defined (OFXIMGUI_FORCE_OF_BACKEND)

	// Vulkan support ?
	#if defined (OF_TARGET_API_VULKAN) && FALSE // tmp disabled, doesn't work either. Maybe rather implement OF backend with vulkan support
		#define OFXIMGUI_LOADED_BACKEND "Vulkan"
		#define OFXIMGUI_BACKEND_VULKAN
		// Warn Vulkan users
		#pragma error "Sorry, there's no tested support for Vulkan yet while it should be very easy to implement"
		// See https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_vulkan.h or ImGui_ImplGlfw_InitForVulkan

	// Global condition for GLES platforms. Todo: Check if there are no false positives ?
	#elif defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW) && FALSE  // tmp disabled, doesn't work either. Now we use the OF backend with gles support
		#define OFXIMGUI_LOADED_BACKEND "GLES"
		#define OFXIMGUI_BACKEND_GLES
		#warning "The GLES backend is an ancient relic, proceed with caution."
		//#include "EngineOpenGLES.h"

	// Platforms where GLFW is default don't define TARGET_GLFW_WINDOW, so we have to explictly add them
	#elif defined(TARGET_GLFW_WINDOW) || (!defined(TARGET_OPENGLES) && (defined(TARGET_LINUX) || defined(TARGET_OSX)  || defined(TARGET_WIN32)))
		OFXIMGUI_COMPILER_MESSAGE("ofxImGui is compiling with the GLFW platform backend.")
		//#include "EngineGLFW.h"
		#define OFXIMGUI_LOADED_BACKEND "GLFW"
		#define OFXIMGUI_BACKEND_GLFW
	#endif

// Default backend : openframeworks
#else
	OFXIMGUI_COMPILER_MESSAGE("ofxImGui is compiling with the openframeworks backend.")
	#define OFXIMGUI_LOADED_BACKEND "OpenFrameworks"
	#define OFXIMGUI_BACKEND_OPENFRAMEWORKS
	// Todo : fallback to engine openframeworks ?
	//#pragma error "Sorry, could not configure ofxImGui on your openFrameworks configuration, nobody ever tried ofxImGui in your config. Feel free to as questions or submit a PR about support for your platform, we'd like to support all Platforms."
#endif

// Renderer selection
// - - - - - - - - - -
// GL ES
#if defined(TARGET_OPENGLES)
	#define OFXIMGUI_RENDERER_GLES
	OFXIMGUI_COMPILER_MESSAGE("ofxImGui is compiling with the GL ES renderer support.")

	// Specialisations
	// RPIs have GL ES 1
	#ifdef TARGET_RASPBERRY_PI
		#define OFXIMGUI_RENDERER_GLES_1
	// While ImGui supports GLES3, openFrameworks doesn't, so let's stick to GLES 2
	#else
		#define OFXIMGUI_RENDERER_GLES_2
	#endif
// Vulkan renderer ?
#elif defined (OF_TARGET_API_VULKAN)
	#error "Sorry, ofxImGui doesn't support Vulkan yet. Some works have been done in the past, and ImGui supports it, if you have OF+Vulkan, feel free to have a look or ask questions."
// GL SL (default)
#else
	#define OFXIMGUI_RENDERER_GLSL
	OFXIMGUI_COMPILER_MESSAGE("ofxImGui is compiling with the GL SL renderer support.")
#endif

// Sanitize GLFW callback flags which allow interfacing GLFW in different ways.
// if 1, it fixes viewports in multiple contexts (one per ofAppWindow)
// if 0, it uses the native imgui backend and multi-ofAppBaseWindows with pop-out windows (viewports) will fail.
#ifdef OFXIMGUI_BACKEND_GLFW
	#ifndef INTERCEPT_GLFW_CALLBACKS
		// You can toggle these lines to change the default value (enabled by default)
		#define INTERCEPT_GLFW_CALLBACKS 1
		//#define INTERCEPT_GLFW_CALLBACKS 0
	#endif
	// Enable Multi-context support by default
	#ifndef IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT
		#define IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT 1
	#endif
#else
	// Always off in non-GLFW contexts
	#define INTERCEPT_GLFW_CALLBACKS 0
	// Disable GLWF multi-context hack
	#define IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT 0
#endif


