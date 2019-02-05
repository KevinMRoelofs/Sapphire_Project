#include "InterfaceElement.h"
#include "../Interface.h"
#include <Editor/Dependencies/imgui-master/imgui.h>

namespace Sapphire
{
	InterfaceElement::InterfaceElement()
	{
		name_ = static_cast<char*>("NewElement");
		width_ = 100;
		height_ = 100;
		positionX_ = 0;
		positionY_ = 0;
		flags_ = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
		interface_.interfaceElements_.push_back(this);
	}

	InterfaceElement::~InterfaceElement()
	{
	}

	void InterfaceElement::Start()
	{
		ImGui::Begin(name_, (bool*)NULL, flags_);
		ImGui::SetWindowSize(ImVec2(float(width_), float(height_)));
		ImGui::SetWindowPos(ImVec2(float(positionX_), float(positionY_)));
		cursorHover = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		drawlist_ = ImGui::GetWindowDrawList();
	}

	void InterfaceElement::End()
	{
		ImGui::End();
	}
}
