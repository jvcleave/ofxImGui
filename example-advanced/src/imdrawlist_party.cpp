//-----------------------------------------------------------------------------
// ImDrawList coding Party!
// v0.23
// Paste this anywhere in an imgui app/example and call FxTestBed()
//-----------------------------------------------------------------------------

// Rules:
// - Your source file (fx.inl) must be <= 1024 bytes.
// - Effect should be reasonably portable (not relying on specific render backend callback)
// - OK to use preprocessor define or other tricks as long as they are reasonably portable.
// - Included are: math.h, imgui.h, imgui_internal.h with ImVec2 maths operators
// - The effect should not use ImGui:: functions, only use ImDrawList facilities. No ImGui:: function!
// - Canvas ratio is expected to be 16/9, canvas size expected to be 320 by 180.
// - For simplicity we assume you can encode a color as 0xAAGGBBRR instead of using the IM_COL32() macro,
//   therefore IMGUI_USE_BGRA_PACKED_COLOR config option is not supported!

// Changelog:
// 0.23 - fix mouse y position again (broken in 0.22)
// 0.22 - use InvisibleButton instead of Dummy to capture inputs
// 0.21 - fix mouse y position
// 0.20 - added mouse buttons, added math operators

#include <math.h>
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS // Access to math operators
#include "imgui_internal.h"

// Function signature:
//  void FX(ImDrawList* d, ImVec2 a, ImVec2 b, ImVec2 sz, ImVec4 mouse, float t);
//     d : draw list
//     a : upper-left corner
//     b : lower-right corner
//    sz : size (== b - a)
// mouse : x,y = mouse position (normalized so 0,0 over 'a'; 1,1 is over 'b', not clamped)
//         z,w = left/right button held. <-1.0f not pressed, 0.0f just pressed, >0.0f time held.
//    t  : time
// If not using a given parameter, you can omit its name in your function to save a few characters.

// Insert your code in fx.inl
// This is the file which size we are measuring, and should be kept <1024 bytes
#include "fx.inl"

// Shared testbed
void FxTestBed(bool* _pOpen=nullptr)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("FX", _pOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImVec2 size(320.0f, 180.0f);
    ImGui::InvisibleButton("canvas", size);
    ImVec2 p0 = ImGui::GetItemRectMin();
    ImVec2 p1 = ImGui::GetItemRectMax();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRect(p0, p1);

    ImVec4 mouse_data;
    mouse_data.x = (io.MousePos.x - p0.x) / size.x;
    mouse_data.y = (io.MousePos.y - p0.y) / size.y;
    mouse_data.z = io.MouseDownDuration[0];
    mouse_data.w = io.MouseDownDuration[1];

    FX(draw_list, p0, p1, size, mouse_data, (float)ImGui::GetTime());
    draw_list->PopClipRect();
    ImGui::End();
}

//-----------------------------------------------------------------------------
