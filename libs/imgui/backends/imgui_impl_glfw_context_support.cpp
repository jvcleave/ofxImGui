
//#pragma once

#include "imgui_impl_glfw_context_support.h"
#include "ofxImGuiConstants.h" // To get the definition of OFXIMGUI_GLFW_FIX_MULTICONTEXT_PRIMARY_VP

#if defined(OFXIMGUI_BACKEND_GLFW) && (OFXIMGUI_GLFW_FIX_MULTICONTEXT_SECONDARY_VP == 1)

#include "backends/imgui_impl_glfw.h"
#include "LinkedList.hpp"

// Fwd declarations
class ImGui_ImplGlfw_Data;

// Some global vars
LinkedList<GLFWwindow, ImGuiContext*> ImGui_ImplGlfw_ScopedContext::Contexts = {};

#else

#endif
