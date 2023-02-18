// Fixes for enabling multi-context support for the GLFW backend
// See https://github.com/ocornut/imgui/pull/3934#issuecomment-873213161
// See https://github.com/ocornut/imgui/issues/5439

// Multi-context is temporary in ofxImGui until ImGui eventually implements "additional host viewports"
// Creating and managing multiple contexts is temporary and not recommended by Omar.

// Glfw events are a bit messed; this ensures that the right windows events call upon the right imgui contexts. (see PR#3934 )
// This file aims to reduce changes in the GLFW backend while injecting context switching before imgui callbacks are triggered.

#pragma once

#include "LinkedList.hpp"
#include "backends/imgui_impl_glfw.h"

// Fwd declarations
class ImGui_ImplGlfw_Data;

// Some global vars
static LinkedList<GLFWwindow, ImGuiContext> ImGui_ImplGlfw_Contexts;

// A scoped struct to set the correct context and restores it when destroyed
// Reduces
struct ImGui_ImplGlfw_ScopedContext {
	public:
		inline ImGui_ImplGlfw_ScopedContext(GLFWwindow* window) : prevContext(ImGui::GetCurrentContext()) {
			ImGuiContext* context = ImGui_ImplGlfw_Contexts.findData(window);
			if(context){
				ImGui::SetCurrentContext(context);
			}
		}

		inline ~ImGui_ImplGlfw_ScopedContext(){
			if(prevContext){
				ImGui::SetCurrentContext(prevContext);
			}
		}
	private:
		ImGuiContext* const prevContext;
};

inline void ImGui_ImplGlfw_RegisterWindowContext(GLFWwindow* window, ImGuiContext* context)
{
	ImGui_ImplGlfw_Contexts.add( window, context );
}
inline void ImGui_ImplGlfw_RemoveWindowContext(GLFWwindow* window)
{
	ImGui_ImplGlfw_Contexts.remove(window);
}

// Todo: set context for :
// - ImGui_ImplGlfw_UpdateKeyModifiers ?
// - ImGui_ImplGlfw_MonitorCallback ?
