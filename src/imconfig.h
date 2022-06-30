//-----------------------------------------------------------------------------
// COMPILE-TIME OPTIONS FOR DEAR IMGUI
// Runtime options (clipboard callbacks, enabling various features, etc.) can generally be set via the ImGuiIO structure.
// You can use ImGui::SetAllocatorFunctions() before calling ImGui::CreateContext() to rewire memory allocation functions.
//-----------------------------------------------------------------------------
// A) You may edit imconfig.h (and not overwrite it when updating Dear ImGui, or maintain a patch/rebased branch with your modifications to it)
// B) or '#define IMGUI_USER_CONFIG "my_imgui_config.h"' in your project and then add directives in your own file without touching this template.
//-----------------------------------------------------------------------------
// You need to make sure that configuration settings are defined consistently _everywhere_ Dear ImGui is used, which include the imgui*.cpp
// files but also _any_ of your code that uses Dear ImGui. This is because some compile-time options have an affect on data structures.
// Defining those options in imconfig.h will ensure every compilation unit gets to see the same data structure layouts.
// Call IMGUI_CHECKVERSION() from your .cpp files to verify that the data structures your files are using are matching the ones imgui.cpp is using.
//-----------------------------------------------------------------------------

#pragma once

// USE the OF GL loader (GLEW), otherwise it will autodetect the system glew headers
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM "GL/glew.h"

//---- Define assertion handler. Defaults to calling assert().
// If your macro uses multiple statements, make sure is enclosed in a 'do { .. } while (0)' block so it can be used as a single statement.
//#define IM_ASSERT(_EXPR)  MyAssert(_EXPR)
//#define IM_ASSERT(_EXPR)  ((void)(_EXPR))     // Disable asserts

//---- Define attributes of all API symbols declarations, e.g. for DLL under Windows
// Using Dear ImGui via a shared library is not recommended, because of function call overhead and because we don't guarantee backward nor forward ABI compatibility.
// DLL users: heaps and globals are not shared across DLL boundaries! You will need to call SetCurrentContext() + SetAllocatorFunctions()
// for each static/DLL boundary you are calling from. Read "Context and Memory Allocators" section of imgui.cpp for more details.
//#define IMGUI_API __declspec( dllexport )
//#define IMGUI_API __declspec( dllimport )

//---- Don't define obsolete functions/enums/behaviors. Consider enabling from time to time after updating to avoid using soon-to-be obsolete function/names.
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
//#define IMGUI_DISABLE_OBSOLETE_KEYIO                      // 1.87: disable legacy io.KeyMap[]+io.KeysDown[] in favor io.AddKeyEvent(). This will be folded into IMGUI_DISABLE_OBSOLETE_FUNCTIONS in a few versions.

//---- Disable all of Dear ImGui or don't implement standard windows/tools.
// It is very strongly recommended to NOT disable the demo windows and debug tool during development. They are extremely useful in day to day work. Please read comments in imgui_demo.cpp.
//#define IMGUI_DISABLE                                     // Disable everything: all headers and source files will be empty.
//#define IMGUI_DISABLE_DEMO_WINDOWS                        // Disable demo windows: ShowDemoWindow()/ShowStyleEditor() will be empty.
//#define IMGUI_DISABLE_DEBUG_TOOLS                         // Disable metrics/debugger and other debug tools: ShowMetricsWindow(), ShowDebugLogWindow() and ShowStackToolWindow() will be empty (this was called IMGUI_DISABLE_METRICS_WINDOW before 1.88).

//---- Don't implement some functions to reduce linkage requirements.
//#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS   // [Win32] Don't implement default clipboard handler. Won't use and link with OpenClipboard/GetClipboardData/CloseClipboard etc. (user32.lib/.a, kernel32.lib/.a)
//#define IMGUI_ENABLE_WIN32_DEFAULT_IME_FUNCTIONS          // [Win32] [Default with Visual Studio] Implement default IME handler (require imm32.lib/.a, auto-link for Visual Studio, -limm32 on command-line for MinGW)
//#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS         // [Win32] [Default with non-Visual Studio compilers] Don't implement default IME handler (won't require imm32.lib/.a)
//#define IMGUI_DISABLE_WIN32_FUNCTIONS                     // [Win32] Won't use and link with any Win32 function (clipboard, ime).
//#define IMGUI_ENABLE_OSX_DEFAULT_CLIPBOARD_FUNCTIONS      // [OSX] Implement default OSX clipboard handler (need to link with '-framework ApplicationServices', this is why this is not the default).
//#define IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS            // Don't implement ImFormatString/ImFormatStringV so you can implement them yourself (e.g. if you don't want to link with vsnprintf)
//#define IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS              // Don't implement ImFabs/ImSqrt/ImPow/ImFmod/ImCos/ImSin/ImAcos/ImAtan2 so you can implement them yourself.
//#define IMGUI_DISABLE_FILE_FUNCTIONS                      // Don't implement ImFileOpen/ImFileClose/ImFileRead/ImFileWrite and ImFileHandle at all (replace them with dummies)
//#define IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS              // Don't implement ImFileOpen/ImFileClose/ImFileRead/ImFileWrite and ImFileHandle so you can implement them yourself if you don't want to link with fopen/fclose/fread/fwrite. This will also disable the LogToTTY() function.
//#define IMGUI_DISABLE_DEFAULT_ALLOCATORS                  // Don't implement default allocators calling malloc()/free() to avoid linking with them. You will need to call ImGui::SetAllocatorFunctions().
//#define IMGUI_DISABLE_SSE                                 // Disable use of SSE intrinsics even if available

//---- Include imgui_user.h at the end of imgui.h as a convenience
//#define IMGUI_INCLUDE_IMGUI_USER_H

//---- Pack colors to BGRA8 instead of RGBA8 (to avoid converting from one to another)
//#define IMGUI_USE_BGRA_PACKED_COLOR

//---- Use 32-bit for ImWchar (default is 16-bit) to support unicode planes 1-16. (e.g. point beyond 0xFFFF like emoticons, dingbats, symbols, shapes, ancient languages, etc...)
//#define IMGUI_USE_WCHAR32

//---- Avoid multiple STB libraries implementations, or redefine path/filenames to prioritize another version
// By default the embedded implementations are declared static and not available outside of Dear ImGui sources files.
//#define IMGUI_STB_TRUETYPE_FILENAME   "my_folder/stb_truetype.h"
//#define IMGUI_STB_RECT_PACK_FILENAME  "my_folder/stb_rect_pack.h"
//#define IMGUI_STB_SPRINTF_FILENAME    "my_folder/stb_sprintf.h"    // only used if enabled
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION

//---- Use stb_sprintf.h for a faster implementation of vsnprintf instead of the one from libc (unless IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS is defined)
// Compatibility checks of arguments and formats done by clang and GCC will be disabled in order to support the extra formats provided by stb_sprintf.h.
//#define IMGUI_USE_STB_SPRINTF

//---- Use FreeType to build and rasterize the font atlas (instead of stb_truetype which is embedded by default in Dear ImGui)
// Requires FreeType headers to be available in the include path. Requires program to be compiled with 'misc/freetype/imgui_freetype.cpp' (in this repository) + the FreeType library (not provided).
// On Windows you may use vcpkg with 'vcpkg install freetype --triplet=x64-windows' + 'vcpkg integrate install'.
//#define IMGUI_ENABLE_FREETYPE

//---- Use stb_truetype to build and rasterize the font atlas (default)
// The only purpose of this define is if you want force compilation of the stb_truetype backend ALONG with the FreeType backend.
//#define IMGUI_ENABLE_STB_TRUETYPE

//---- Define constructor and implicit cast operators to convert back<>forth between your math types and ImVec2/ImVec4.
// This will be inlined as part of ImVec2 and ImVec4 class declarations.
// Note: ofVec3 can cast to ImVec2 by ignoring z axis too.
#include "ofVectorMath.h"
#include "ofColor.h"

constexpr float ofColorScale(1.0f/255.0f);
#define IM_VEC2_CLASS_EXTRA_OF_9 \
    constexpr ImVec2(const ofVec2f& f) : x(f.x), y(f.y) {}   \
    operator ofVec2f() const { return ofVec2f(x, y); }       \
    constexpr ImVec2(const glm::vec2& f) : x(f.x), y(f.y) {} \
    constexpr ImVec2(const ofVec3f& f) : x(f.x), y(f.y) {}   \

// OF >= 0.10 has ofVec2 = glm::vec2 --> bind aliases
#if OF_VERSION_MINOR >= 10
#define IM_VEC2_CLASS_EXTRA_OF_10 \
    operator glm::vec2() const { return glm::vec2(x, y); }   \
    constexpr ImVec2(const glm::vec3& f) : x(f.x), y(f.y) {}
#else
#define IM_VEC2_CLASS_EXTRA_OF_10 // empty
#endif

#define IM_VEC2_CLASS_EXTRA IM_VEC2_CLASS_EXTRA_OF_9 IM_VEC2_CLASS_EXTRA_OF_10

// Todo: Add ofVec3 -> ImVec4 (assuming alpha=100%) ??? (works for colors, maybe not rectangles....
#define IM_VEC4_CLASS_EXTRA_OF_9 \
    constexpr ImVec4(const ofVec4f& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}                                                                       \
    operator ofVec4f() const { return ofVec4f(x,y,z,w); }                                                                                        \
    constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}                                                                     \
    constexpr ImVec4(const ofColor& color, float alpha) : x(color.r*ofColorScale), y(color.g*ofColorScale), z(color.b*ofColorScale), w(alpha) {} \
    constexpr ImVec4(const ofColor& f) : x(f.r*ofColorScale), y(f.g*ofColorScale), z(f.b*ofColorScale), w(f.a*ofColorScale) {}                   \
    operator ofColor() const { return ofColor((int) (x*255.0f+0.5f), (int) (y*255.0f+0.5f), (int) (z*255.0f+0.5f), (int) (w*255.0f+0.5f)); }     \
    constexpr ImVec4(const ofFloatColor& color, float alpha) : x(color.r), y(color.g), z(color.b), w(alpha) {}                                   \
    constexpr ImVec4(const ofFloatColor& f) : x(f.r), y(f.g), z(f.b), w(f.a) {}                                                                  \
    operator ofFloatColor() const { return ofFloatColor(x, y, z, w); }

// OF >= 0.10 has ofVec4 = glm::vec4 --> bind aliases
#if OF_VERSION_MINOR >= 10
#define IM_VEC4_CLASS_EXTRA_OF_10 \
    operator glm::vec4() const { return glm::vec4(x,y,z,w); }
#else
#define IM_VEC4_CLASS_EXTRA_OF_10 // empty
#endif
#define IM_VEC4_CLASS_EXTRA IM_VEC4_CLASS_EXTRA_OF_9 IM_VEC4_CLASS_EXTRA_OF_10


#define ImDrawIdx ofIndexType


//---- Use 32-bit vertex indices (default is 16-bit) is one way to allow large meshes with more than 64K vertices.
// Your renderer backend will need to support it (most example renderer backends support both 16/32-bit indices).
// Another way to allow large meshes while keeping 16-bit indices is to handle ImDrawCmd::VtxOffset in your renderer.
// Read about ImGuiBackendFlags_RendererHasVtxOffset for details.
//#define ImDrawIdx unsigned int

//---- Override ImDrawCallback signature (will need to modify renderer backends accordingly)
//struct ImDrawList;
//struct ImDrawCmd;
//typedef void (*MyImDrawCallback)(const ImDrawList* draw_list, const ImDrawCmd* cmd, void* my_renderer_user_data);
//#define ImDrawCallback MyImDrawCallback

//---- Debug Tools: Macro to break in Debugger
// (use 'Metrics->Tools->Item Picker' to pick widgets with the mouse and break into them for easy debugging.)
//#define IM_DEBUG_BREAK  IM_ASSERT(0)
//#define IM_DEBUG_BREAK  __debugbreak()

//---- Debug Tools: Have the Item Picker break in the ItemAdd() function instead of ItemHoverable(),
// (which comes earlier in the code, will catch a few extra items, allow picking items other than Hovered one.)
// This adds a small runtime cost which is why it is not enabled by default.
//#define IMGUI_DEBUG_TOOL_ITEM_PICKER_EX

//---- Debug Tools: Enable slower asserts
//#define IMGUI_DEBUG_PARANOID

//---- Tip: You can add extra functions within the ImGui:: namespace, here or in your own headers files.
/*
namespace ImGui
{
    void MyFunction(const char* name, const MyMatrix44& v);
}
*/

//#define IMGUI_SET_CURRENT_CONTEXT_FUNC(ctx) { GImGui = ctx; }// g_Window = io.BackendPlatformUserData; }

// Note: maybe other variables need to be stitched too ?
//#define IMGUI_BACKEND_GLFW_CUSTOM_NEWFRAME() { bd->Window = (GLFWwindow*)io.BackendPlatformUserData; }
