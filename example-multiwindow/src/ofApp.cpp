#include "ofApp.h"

namespace ImGuiEx {

	void ShowHelpMarker(const char* desc){
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void ShowWarningMarker(const char* desc){
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(255,100,0,255));
		ImGui::TextDisabled("/!\\");
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}
