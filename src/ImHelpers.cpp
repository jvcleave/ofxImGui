#include "ImHelpers.h"

//--------------------------------------------------------------
ofxImGui::Settings::Settings()
	: windowPos(kImGuiMargin, kImGuiMargin)
	, windowSize(ofVec2f::zero())
	, lockPosition(false)
	, windowBlock(false)
	, mouseOverGui(false)
    , treeLevel(0)
{}

bool ofxImGui::IsMouseOverGui()
{
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}

//--------------------------------------------------------------
const char * ofxImGui::GetUniqueName(ofAbstractParameter& parameter)
{
	return GetUniqueName(parameter.getName());
}

//--------------------------------------------------------------
const char * ofxImGui::GetUniqueName(const std::string& candidate)
{
	std::string result = candidate;
	while (std::find(windowOpen.usedNames.top().begin(), windowOpen.usedNames.top().end(), result) != windowOpen.usedNames.top().end())
	{
		result += " ";
	}
	windowOpen.usedNames.top().push_back(result);
	return windowOpen.usedNames.top().back().c_str();
}

//--------------------------------------------------------------
void ofxImGui::SetNextWindow(Settings& settings)
{
	settings.windowSize.x = 0;
	settings.windowPos.y += settings.windowSize.y + kImGuiMargin;
}

//--------------------------------------------------------------
bool ofxImGui::BeginWindow(ofParameter<bool>& parameter, Settings& settings, bool collapse)
{
	if (settings.windowBlock)
	{
		ofLogWarning(__FUNCTION__) << "Already inside a window block!";
		return false;
	}

	// Reference this ofParameter until EndWindow().
	windowOpen.parameter = std::dynamic_pointer_cast<ofParameter<bool>>(parameter.newReference());
	windowOpen.value = parameter.get();

	auto result = ofxImGui::BeginWindow(parameter.getName(), settings, collapse, &windowOpen.value);
	parameter = windowOpen.value;
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::BeginWindow(const std::string& name, Settings& settings, bool collapse, bool * open)
{
	if (settings.windowBlock)
	{
		ofLogWarning(__FUNCTION__) << "Already inside a window block!";
		return false;
	}

	settings.windowBlock = true;

	// Push a new list of names onto the stack.
	windowOpen.usedNames.push(std::vector<std::string>());

	ImGui::SetNextWindowPos(settings.windowPos, settings.lockPosition? ImGuiCond_Always : ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(settings.windowSize, ImGuiCond_Appearing);
	//ImGui::SetNextWindowCollapsed(collapse, ImGuiCond_Appearing);
	return ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | (collapse ? 0 : ImGuiWindowFlags_NoCollapse));
}

//--------------------------------------------------------------
bool ofxImGui::BeginWindow(const std::string& name, Settings& settings, ImGuiWindowFlags flags, bool * open)
{
	if (settings.windowBlock)
	{
		ofLogWarning(__FUNCTION__) << "Already inside a window block!";
		return false;
	}

	settings.windowBlock = true;

	// Push a new list of names onto the stack.
	windowOpen.usedNames.push(std::vector<std::string>());

	ImGui::SetNextWindowPos(settings.windowPos, settings.lockPosition? ImGuiCond_Always : ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(settings.windowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowCollapsed(!(flags & ImGuiWindowFlags_NoCollapse), ImGuiCond_Appearing);
	return ImGui::Begin(name.c_str(), open, flags);
}

//--------------------------------------------------------------
void ofxImGui::EndWindow(Settings& settings)
{
	if (!settings.windowBlock)
	{
		ofLogWarning(__FUNCTION__) << "Not inside a window block!";
		return;
	}

	settings.windowBlock = false;

	settings.windowPos = ImGui::GetWindowPos();
	settings.windowSize = ImGui::GetWindowSize();
	settings.mouseOverGui |= ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	ImGui::End();

	// Unlink the referenced ofParameter.
	windowOpen.parameter.reset();

	// Clear the list of names from the stack.
	windowOpen.usedNames.pop();

	// Include this window's bounds in the total bounds.
	const auto windowBounds = ofRectangle(settings.windowPos, settings.windowSize.x, settings.windowSize.y);
	if (settings.totalBounds.isZero())
	{
		settings.totalBounds = windowBounds;
	}
	else
	{
		settings.totalBounds.growToInclude(windowBounds);
	}
}

//--------------------------------------------------------------
bool ofxImGui::BeginTree(ofAbstractParameter& parameter, Settings& settings)
{
	return ofxImGui::BeginTree(parameter.getName(), settings);
}

//--------------------------------------------------------------
bool ofxImGui::BeginTree(const std::string& name, Settings& settings)
{
	bool result;
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (settings.treeLevel == 0)
	{
		result = ImGui::TreeNodeEx(GetUniqueName(name), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog);
	}
	else
	{
		result = ImGui::TreeNode(GetUniqueName(name));
	}
	if (result)
	{
		settings.treeLevel += 1;

		// Push a new list of names onto the stack.
		windowOpen.usedNames.push(std::vector<std::string>());
	}
	return result;
}

//--------------------------------------------------------------
void ofxImGui::EndTree(Settings& settings)
{
	ImGui::TreePop();
	
	settings.treeLevel = std::max(0, settings.treeLevel - 1);

	// Clear the list of names from the stack.
	windowOpen.usedNames.pop();
}

//--------------------------------------------------------------
void ofxImGui::AddGroup(ofParameterGroup& group, Settings& settings)
{
	bool prevWindowBlock = settings.windowBlock;
	if (settings.windowBlock)
	{
		if (!ofxImGui::BeginTree(group, settings))
		{
			return;
		}
	}
	else
	{
		if (!ofxImGui::BeginWindow(group.getName().c_str(), settings))
		{
			ofxImGui::EndWindow(settings);
			return;
		}
	}

	for (auto parameter : group)
	{
		// Group.
		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(parameter);
		if (parameterGroup)
		{
			// Recurse through contents.
			ofxImGui::AddGroup(*parameterGroup, settings);
			continue;
		}

		// Parameter, try everything we know how to handle.
#if OF_VERSION_MINOR >= 10
		auto parameterVec2f = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(parameter);
		if (parameterVec2f)
		{
			ofxImGui::AddParameter(*parameterVec2f);
			continue;
		}
		auto parameterVec3f = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(parameter);
		if (parameterVec3f)
		{
			ofxImGui::AddParameter(*parameterVec3f);
			continue;
		}
		auto parameterVec4f = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(parameter);
		if (parameterVec4f)
		{
			ofxImGui::AddParameter(*parameterVec4f);
			continue;
		}
#endif
		auto parameterOfVec2f = std::dynamic_pointer_cast<ofParameter<ofVec2f>>(parameter);
		if (parameterOfVec2f)
		{
			ofxImGui::AddParameter(*parameterOfVec2f);
			continue;
		}
		auto parameterOfVec3f = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(parameter);
		if (parameterOfVec3f)
		{
			ofxImGui::AddParameter(*parameterOfVec3f);
			continue;
		}
		auto parameterOfVec4f = std::dynamic_pointer_cast<ofParameter<ofVec4f>>(parameter);
		if (parameterOfVec4f)
		{
			ofxImGui::AddParameter(*parameterOfVec4f);
			continue;
		}
		auto parameterFloatColor = std::dynamic_pointer_cast<ofParameter<ofFloatColor>>(parameter);
		if (parameterFloatColor)
		{
			ofxImGui::AddParameter(*parameterFloatColor);
			continue;
		}
		auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(parameter);
		if (parameterFloat)
		{
			ofxImGui::AddParameter(*parameterFloat);
			continue;
		}
		auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(parameter);
		if (parameterInt)
		{
			ofxImGui::AddParameter(*parameterInt);
			continue;
		}
		auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(parameter);
		if (parameterBool)
		{
			ofxImGui::AddParameter(*parameterBool);
			continue;
		}

		ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << parameter->getName();
	}

	if (settings.windowBlock && !prevWindowBlock)
	{
		// End window if we created it.
		ofxImGui::EndWindow(settings);
	}
	else
	{
		// End tree.
		ofxImGui::EndTree(settings);
	}
}

#if OF_VERSION_MINOR >= 10

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::tvec2<int>>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderInt2(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::tvec3<int>>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderInt3(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::tvec4<int>>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderInt4(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::vec2>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat2(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::vec3>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat3(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<glm::vec4>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat4(GetUniqueName(parameter), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

#endif

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<ofVec2f>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat2(GetUniqueName(parameter), tmpRef.getPtr(), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<ofVec3f>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat3(GetUniqueName(parameter), tmpRef.getPtr(), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<ofVec4f>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat4(GetUniqueName(parameter), tmpRef.getPtr(), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<ofFloatColor>& parameter, bool alpha)
{
	auto tmpRef = parameter.get();
	if (alpha)
	{
		if (ImGui::ColorEdit4(GetUniqueName(parameter), &tmpRef.r))
		{
			parameter.set(tmpRef);
			return true;
		}
	}
	else if (ImGui::ColorEdit3(GetUniqueName(parameter), &tmpRef.r))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<std::string>& parameter, size_t maxChars, bool multiline)
{
	auto tmpRef = parameter.get();
	char * cString = new char[maxChars];
	strcpy(cString, tmpRef.c_str());
	auto result = false;
	if (multiline)
	{
		if (ImGui::InputTextMultiline(GetUniqueName(parameter), cString, maxChars))
		{
			parameter.set(cString);
			result = true;
		}
	}
	else if (ImGui::InputText(GetUniqueName(parameter), cString, maxChars))
	{
		parameter.set(cString);
		result = true;
	}
	delete[] cString;
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddParameter(ofParameter<void>& parameter, float width)
{
	if (ImGui::Button(GetUniqueName(parameter), glm::vec2(width, 0.0f)))
	{
		parameter.trigger();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddRadio(ofParameter<int>& parameter, std::vector<std::string> labels, int columns)
{
	auto uniqueName = GetUniqueName(parameter);
    ImGui::Text("%s", uniqueName);
	auto result = false;
	auto tmpRef = parameter.get();
	ImGui::PushID(uniqueName);
	{
		ImGui::Columns(columns);
		for (size_t i = 0; i < labels.size(); ++i)
		{
			result |= ImGui::RadioButton(GetUniqueName(labels[i]), &tmpRef, i);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}
	ImGui::PopID();
	if (result)
	{
		parameter.set(tmpRef);
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddCombo(ofParameter<int>& parameter, std::vector<std::string> labels)
{
	auto result = false;
	auto tmpRef = parameter.get();
	if (ImGui::BeginCombo(GetUniqueName(parameter), labels.at(parameter.get()).c_str()))
	{
		for (size_t i = 0; i < labels.size(); ++i)
		{
			bool selected = (i == tmpRef);
			if (ImGui::Selectable(labels[i].c_str(), selected))
			{
				tmpRef = i;
				result = true;
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
	if (result)
	{
		parameter.set(tmpRef);
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddStepper(ofParameter<int>& parameter, int step, int stepFast)
{
	auto tmpRef = parameter.get();
	if (ImGui::InputInt(GetUniqueName(parameter), &tmpRef, step, stepFast))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddSlider(ofParameter<float>& parameter, const char* format, float power)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat(GetUniqueName(parameter), (float*)&tmpRef, parameter.getMin(), parameter.getMax(), format, power))
	{
		parameter.set(tmpRef);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddRange(const std::string& name, ofParameter<int>& parameterMin, ofParameter<int>& parameterMax, int speed)
{
	auto tmpRefMin = parameterMin.get();
	auto tmpRefMax = parameterMax.get();
	if (ImGui::DragIntRange2(GetUniqueName(name), &tmpRefMin, &tmpRefMax, speed, parameterMin.getMin(), parameterMax.getMax()))
	{
		parameterMin.set(tmpRefMin);
		parameterMax.set(tmpRefMax);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddRange(const std::string& name, ofParameter<float>& parameterMin, ofParameter<float>& parameterMax, float speed)
{
	auto tmpRefMin = parameterMin.get();
	auto tmpRefMax = parameterMax.get();
	if (ImGui::DragFloatRange2(GetUniqueName(name), &tmpRefMin, &tmpRefMax, speed, parameterMin.getMin(), parameterMax.getMax()))
	{
		parameterMin.set(tmpRefMin);
		parameterMax.set(tmpRefMax);
		return true;
	}
	return false;
}

#if OF_VERSION_MINOR >= 10

//--------------------------------------------------------------
bool ofxImGui::AddRange(const std::string& name, ofParameter<glm::vec2>& parameterMin, ofParameter<glm::vec2>& parameterMax, float speed)
{
	auto result = false;
	auto tmpRefMin = parameterMin.get();
	auto tmpRefMax = parameterMax.get();
	if (ImGui::DragFloatRange2(GetUniqueName(name + " X"), &tmpRefMin.x, &tmpRefMax.x, speed, parameterMin.getMin().x, parameterMax.getMax().x))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " Y"), &tmpRefMin.y, &tmpRefMax.y, speed, parameterMin.getMin().y, parameterMax.getMax().y))
	{
		result |= true;
	}
	if (result)
	{
		parameterMin.set(tmpRefMin);
		parameterMax.set(tmpRefMax);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddRange(const std::string& name, ofParameter<glm::vec3>& parameterMin, ofParameter<glm::vec3>& parameterMax, float speed)
{
	auto result = false;
	auto tmpRefMin = parameterMin.get();
	auto tmpRefMax = parameterMax.get();
	if (ImGui::DragFloatRange2(GetUniqueName(name + " X"), &tmpRefMin.x, &tmpRefMax.x, speed, parameterMin.getMin().x, parameterMax.getMax().x))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " Y"), &tmpRefMin.y, &tmpRefMax.y, speed, parameterMin.getMin().y, parameterMax.getMax().y))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " Z"), &tmpRefMin.z, &tmpRefMax.z, speed, parameterMin.getMin().z, parameterMax.getMax().z))
	{
		result |= true;
	}
	if (result)
	{
		parameterMin.set(tmpRefMin);
		parameterMax.set(tmpRefMax);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxImGui::AddRange(const std::string& name, ofParameter<glm::vec4>& parameterMin, ofParameter<glm::vec4>& parameterMax, float speed)
{
	auto result = false;
	auto tmpRefMin = parameterMin.get();
	auto tmpRefMax = parameterMax.get();
	if (ImGui::DragFloatRange2(GetUniqueName(name + " X"), &tmpRefMin.x, &tmpRefMax.x, speed, parameterMin.getMin().x, parameterMax.getMax().x))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " Y"), &tmpRefMin.y, &tmpRefMax.y, speed, parameterMin.getMin().y, parameterMax.getMax().y))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " Z"), &tmpRefMin.z, &tmpRefMax.z, speed, parameterMin.getMin().z, parameterMax.getMax().z))
	{
		result |= true;
	}
	if (ImGui::DragFloatRange2(GetUniqueName(name + " W"), &tmpRefMin.w, &tmpRefMax.w, speed, parameterMin.getMin().w, parameterMax.getMax().w))
	{
		result |= true;
	}
	if (result)
	{
		parameterMin.set(tmpRefMin);
		parameterMax.set(tmpRefMax);
		return true;
	}
	return false;
}

#endif

#if OF_VERSION_MINOR >= 10

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::tvec2<int>>& values, int minValue, int maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragInt2(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderInt2(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::tvec3<int>>& values, int minValue, int maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragInt3(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderInt3(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::tvec4<int>>& values, int minValue, int maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragInt4(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderInt4(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::vec2>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat2(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderFloat2(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::vec3>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat3(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderFloat3(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<glm::vec4>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat4(GetUniqueName(iname), glm::value_ptr(values[i]));
		}
		else
		{
			result |= ImGui::SliderFloat4(GetUniqueName(iname), glm::value_ptr(values[i]), minValue, maxValue);
		}
	}
	return result;
}

#endif

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<ofVec2f>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat2(GetUniqueName(iname), values[i].getPtr());
		}
		else
		{
			result |= ImGui::SliderFloat2(GetUniqueName(iname), values[i].getPtr(), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<ofVec3f>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat3(GetUniqueName(iname), values[i].getPtr());
		}
		else
		{
			result |= ImGui::SliderFloat3(GetUniqueName(iname), values[i].getPtr(), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
bool ofxImGui::AddValues(const std::string& name, std::vector<ofVec4f>& values, float minValue, float maxValue)
{
	auto result = false;
	for (size_t i = 0; i < values.size(); ++i)
	{
		const auto iname = name + " " + ofToString(i);
		if (minValue == 0 && maxValue == 0)
		{
			result |= ImGui::DragFloat4(GetUniqueName(iname), values[i].getPtr());
		}
		else
		{
			result |= ImGui::SliderFloat4(GetUniqueName(iname), values[i].getPtr(), minValue, maxValue);
		}
	}
	return result;
}

//--------------------------------------------------------------
void ofxImGui::AddImage(const ofBaseHasTexture& hasTexture, const ofVec2f& size)
{
	ofxImGui::AddImage(hasTexture.getTexture(), size);
}

//--------------------------------------------------------------
void ofxImGui::AddImage(const ofTexture& texture, const ofVec2f& size)
{
	ImTextureID textureID = GetImTextureID(texture);
	ImGui::Image(textureID, size);
}

#if OF_VERSION_MINOR >= 10

//--------------------------------------------------------------
void ofxImGui::AddImage(const ofBaseHasTexture& hasTexture, const glm::vec2& size)
{
    ofxImGui::AddImage(hasTexture.getTexture(), size);
}

//--------------------------------------------------------------
void ofxImGui::AddImage(const ofTexture& texture, const glm::vec2& size)
{
    ImTextureID textureID = GetImTextureID(texture);
    ImGui::Image(textureID, size);
}

#endif

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
};

bool ofxImGui::VectorCombo(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ImGui::Combo(label, currIndex, vector_getter,
                        static_cast<void*>(&values), values.size());
}

bool ofxImGui::VectorListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ImGui::ListBox(label, currIndex, vector_getter,
                   static_cast<void*>(&values), values.size());
}

