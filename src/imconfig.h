//-----------------------------------------------------------------------------
// USER IMPLEMENTATION
// This file contains compile-time options for ImGui.
// Other options (memory allocation overrides, callbacks, etc.) can be set at runtime via the ImGuiIO structure - ImGui::GetIO().
//-----------------------------------------------------------------------------

#pragma once

//---- Define assertion handler. Defaults to calling assert().
//#define IM_ASSERT(_EXPR)  MyAssert(_EXPR)

//---- Define attributes of all API symbols declarations, e.g. for DLL under Windows.
//#define IMGUI_API __declspec( dllexport )
//#define IMGUI_API __declspec( dllimport )

//---- Include imgui_user.inl at the end of imgui.cpp so you can include code that extends ImGui using its private data/functions.
//#define IMGUI_INCLUDE_IMGUI_USER_INL

//---- Include imgui_user.h at the end of imgui.h
//#define IMGUI_INCLUDE_IMGUI_USER_H

//---- Don't implement default handlers for Windows (so as not to link with OpenClipboard() and others Win32 functions)
//#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS
//#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS

//---- Don't implement help and test window functionality (ShowUserGuide()/ShowStyleEditor()/ShowTestWindow() methods will be empty)
//#define IMGUI_DISABLE_TEST_WINDOWS

//---- Don't define obsolete functions names
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//---- Implement STB libraries in a namespace to avoid conflicts
//#define IMGUI_STB_NAMESPACE     ImGuiStb

//---- Define constructor and implicit cast operators to convert back<>forth from your math types and ImVec2/ImVec4.

#include "ofVectorMath.h"
#include "ofColor.h"

#if OF_VERSION_MINOR >= 10
#define IM_VEC2_CLASS_EXTRA							\
ImVec2(const ofVec2f& f) { x = f.x; y = f.y; }		\
operator ofVec2f() const { return ofVec2f(x, y); }	\
ImVec2(const glm::vec2& f) { x = f.x; y = f.y; }	\
operator glm::vec2() const { return glm::vec2(x, y); }
#else
#define IM_VEC2_CLASS_EXTRA							\
ImVec2(const ofVec2f& f) { x = f.x; y = f.y; }		\
operator ofVec2f() const { return ofVec2f(x, y); }
#endif

#if OF_VERSION_MINOR >= 10
#define IM_VEC4_CLASS_EXTRA																													\
ImVec4(const ofVec4f& f) { x = f.x; y = f.y; z = f.z; w = f.w; }																			\
operator ofVec4f() const { return ofVec4f(x,y,z,w); }																						\
ImVec4(const glm::vec4& f) { x = f.x; y = f.y; z = f.z; w = f.w; }																			\
operator glm::vec4() const { return glm::vec4(x,y,z,w); }																					\
ImVec4(const ofColor& color, float alpha) { static const float sc = 1.0f/255.0f; x = color.r*sc; y = color.g*sc; z = color.b*sc; w = alpha; }		\
ImVec4(const ofColor& f) { static const float sc = 1.0f/255.0f; x = f.r*sc; y = f.g*sc; z = f.b*sc; w = f.a*sc; }							\
operator ofColor() const { return ofColor((int) (x*255.0f+0.5f), (int) (y*255.0f+0.5f), (int) (z*255.0f+0.5f), (int) (w*255.0f+0.5f)); }	\
ImVec4(const ofFloatColor& color, float alpha) { x = color.r; y = color.g; z = color.b; w = alpha; }												\
ImVec4(const ofFloatColor& f) { x = f.r; y = f.g; z = f.b; w = f.a; }																		\
operator ofFloatColor() const { return ofFloatColor(x, y, z, w); }
#else
#define IM_VEC4_CLASS_EXTRA																													\
ImVec4(const ofVec4f& f) { x = f.x; y = f.y; z = f.z; w = f.w; }																			\
operator ofVec4f() const { return ofVec4f(x,y,z,w); }																						\
ImVec4(const ofColor& color, float alpha) { static const float sc = 1.0f/255.0f; x = color.r*sc; y = color.g*sc; z = color.b*sc; w = alpha; }		\
ImVec4(const ofColor& f) { static const float sc = 1.0f/255.0f; x = f.r*sc; y = f.g*sc; z = f.b*sc; w = f.a*sc; }							\
operator ofColor() const { return ofColor((int) (x*255.0f+0.5f), (int) (y*255.0f+0.5f), (int) (z*255.0f+0.5f), (int) (w*255.0f+0.5f)); }	\
ImVec4(const ofFloatColor& color, float alpha) { x = color.r; y = color.g; z = color.b; w = alpha; }												\
ImVec4(const ofFloatColor& f) { x = f.r; y = f.g; z = f.b; w = f.a; }																		\
operator ofFloatColor() const { return ofFloatColor(x, y, z, w); }
#endif

#define ImDrawIdx ofIndexType

//---- Freely implement extra functions within the ImGui:: namespace.
//---- Declare helpers or widgets implemented in imgui_user.inl or elsewhere, so end-user doesn't need to include multiple files.
//---- e.g. you can create variants of the ImGui::Value() helper for your low-level math types, or your own widgets/helpers.
/*
namespace ImGui
{
    void    Value(const char* prefix, const MyVec2& v, const char* float_format = NULL);
    void    Value(const char* prefix, const MyVec4& v, const char* float_format = NULL);
}
*/

