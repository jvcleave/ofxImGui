#pragma once

namespace ImGui {

	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}



	//inline bool InputRect(const char* label, ofRect* rectPtr,
	//	int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0)
	//{
	//	ImGui::PushID(label);
	//	ImGui::BeginGroup();

	//	bool valueChanged = false;

	//	std::array<float*, 4> arr = { &rectPtr->x, &rectPtr->y,
	//		&rectPtr->w, &rectPtr->h };

	//	for (auto& elem : arr) {
	//		ImGui::PushID(elem);
	//		ImGui::PushItemWidth(64.f);
	//		valueChanged |= ImGui::InputFloat("##arr", elem, 0, 0,
	//			decimal_precision, extra_flags);
	//		ImGui::PopID();
	//		ImGui::SameLine();
	//	}

	//	ImGui::SameLine();
	//	ImGui::TextUnformatted(label);
	//	ImGui::EndGroup();

	//	ImGui::PopID(); // pop label id;

	//	return valueChanged;
	//}
}