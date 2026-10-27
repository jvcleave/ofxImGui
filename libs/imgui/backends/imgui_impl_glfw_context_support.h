// Fixes for enabling multi-context support for the GLFW backend
// See https://github.com/ocornut/imgui/pull/3934#issuecomment-873213161
// See https://github.com/ocornut/imgui/issues/5439

// Multi-context is temporary in ofxImGui until ImGui eventually implements "additional host viewports"
// Creating and managing multiple contexts is temporary and not recommended by Omar.

// Glfw events are a bit messed; this ensures that the right windows events call upon the right imgui contexts. (see PR#3934 )
// This file aims to reduce changes in the GLFW backend while injecting context switching before imgui callbacks are triggered.

#pragma once

#include "ofxImGuiConstants.h" // To get the definition of OFXIMGUI_GLFW_FIX_MULTICONTEXT_PRIMARY_VP
#if defined(OFXIMGUI_BACKEND_GLFW) && (OFXIMGUI_GLFW_FIX_MULTICONTEXT_SECONDARY_VP == 1)

#include "backends/imgui_impl_glfw.h"

#include "LinkedList.hpp"

// Fwd declarations
struct ImGui_ImplGlfw_Data;

namespace ofxImGui {
	class Gui;
}

// A scoped struct to set the correct context and restores it when destroyed
// Reduces amount of code changes in original lib code
struct ImGui_ImplGlfw_ScopedContext {
		friend class ofxImGui::Gui;
	public:
		ImGui_ImplGlfw_ScopedContext(GLFWwindow* window) : prevContext(calcPrevContext(window)){

		}
		~ImGui_ImplGlfw_ScopedContext(){
			if(prevContext != nullptr){
				ImGui::SetCurrentContext(prevContext);
			}
		}
		static inline void RegisterWindowContext(GLFWwindow* window, ImGuiContext* context){
			Contexts.add( window, context );
		}
		static inline void RemoveWindowContext(GLFWwindow* window){
			Contexts.remove(window);
		}

	private:
		static ImGuiContext* calcPrevContext(GLFWwindow* window){
			ImGuiContext* prevContext = ImGui::GetCurrentContext();
			if(prevContext){
				ImGuiContext* context = Contexts.findData(window);
				if(context && context != prevContext){
					ImGui::SetCurrentContext(context);
					return prevContext;
				}
			}
			return (ImGuiContext*) nullptr;
		}
		ImGuiContext* const prevContext;
		// Contains all standalone viewport windows.
		static LinkedList<GLFWwindow, ImGuiContext*> Contexts;
};

// Very tmp not to modify imgui_glfw again
inline void ImGui_ImplGlfw_RegisterWindowContext(GLFWwindow* window, ImGuiContext* context){
	ImGui_ImplGlfw_ScopedContext::RegisterWindowContext(window, context);
}
inline void ImGui_ImplGlfw_RemoveWindowContext(GLFWwindow* window){
	ImGui_ImplGlfw_ScopedContext::RemoveWindowContext(window);
}

// Todo: set context for :
// - ImGui_ImplGlfw_UpdateKeyModifiers ?
// - ImGui_ImplGlfw_MonitorCallback ?

// Handle GLFW capabilities hack for OF using in-between GLFW version (non-realease, master branch)
// Corrects the IMGUI detection to the features that the OF version has.
// More info in Developers.md
// Guard with #ifndef so that a system GLFW that already defines these with real
// values (e.g. MSYS2 GLFW >= 3.3) is not silently overridden, which would cause
// a redefinition warning and leave imgui unable to use the feature.
#ifndef GLFW_RESIZE_NESW_CURSOR
#define GLFW_RESIZE_NESW_CURSOR
#endif
#ifndef GLFW_MOUSE_PASSTHROUGH
#define GLFW_MOUSE_PASSTHROUGH
#endif
#ifndef OFXIMGUI_GLFW_NO_VERSION_HACKS
#if OF_VERSION_MAJOR == 0
#	if OF_VERSION_MINOR == 11
#		if OF_VERSION_PATCH == 0 // 0.11.0 has GLFW pre-3.3.0
#			define OFXIMGUI_VERSION_GLFW_3300 3301
#		elif OF_VERSION_PATCH == 1 // 0.11.1 has GLFW 3.3.0
#		elif OF_VERSION_PATCH == 2 // 0.11.2 has GLFW pre-3.3.0
#			define OFXIMGUI_VERSION_GLFW_3300 3301
#		endif
#	elif OF_VERSION_MINOR == 12
#		if OF_VERSION_PATCH == 0 // 0.12.0 has GLFW 3.3.8
#		elif OF_VERSION_PATCH == 1 // 0.12.0 has GLFW 3.4
#		endif
#	endif
#endif
#endif

// Default value
#ifndef OFXIMGUI_VERSION_GLFW_3300
#	define OFXIMGUI_VERSION_GLFW_3300 3300
#endif

#else
// Define classes, compiler should strip all in optimisation steps as it's just dummy code.
class GLFWwindow;
class ImGuiContext;
inline void ImGui_ImplGlfw_RemoveWindowContext(GLFWwindow* window){}
inline void ImGui_ImplGlfw_RegisterWindowContext(GLFWwindow* window, ImGuiContext* context){}
struct ImGui_ImplGlfw_ScopedContext {
		ImGui_ImplGlfw_ScopedContext(GLFWwindow* window){}
};
#endif
