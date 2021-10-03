// created by 
// on 07

#pragma once
#include "imgui.h"
#include "imgui_internal.h"
using namespace ImGui;

namespace LitGui
{
	static IMGUI_API bool ImageButton(ImGuiID id, ImTextureID user_texture_id, const ImVec2& size,
	                                  ImGuiButtonFlags flags_p = 0, const ImVec2& uv0 = ImVec2(0, 0),
	                                  const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1,
	                                  const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
	                                  const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;


		const ImVec2 padding = (frame_padding >= 0)
			                       ? ImVec2(static_cast<float>(frame_padding), static_cast<float>(frame_padding))
			                       : style.FramePadding;
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
		ItemSize(bb);
		if (!ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags_p);

		// Render
		const ImU32 col = GetColorU32((held && hovered)
			                              ? ImGuiCol_ButtonActive
			                              : hovered
			                              ? ImGuiCol_ButtonHovered
			                              : ImGuiCol_Button);
		RenderNavHighlight(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp(ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
		window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

		return pressed;
	}
}
