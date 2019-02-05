#pragma once
#include "InterfaceElement.h"
#include "Engine/Resources/Object.h"
#include <Engine/Dependencies/glm/detail/type_vec2.hpp>
#include <vector>
#include <memory>
#include <Editor/Dependencies/imgui-master/imgui.h>

//struct ImColor;
//class Object;

namespace Sapphire
{
	class Hierarchy : public InterfaceElement
	{
	public:
		Hierarchy();
		~Hierarchy();
		void Start() override;
		void Update() override;
		void End() override;

		void Selecting();
		void SelectionActions();
		void SelectionBox();
		bool Select(std::weak_ptr<Object> selectedObject, bool addToSelection);
		void Deselect(std::weak_ptr<Object> deselectedObject);
		bool IsInSelection(std::weak_ptr<Object> passedObject);

		glm::vec2 cursorPosStart{}, cursorPosCurrent{}, cursorPosLastframe{};
		bool cursorDraggingSelObject{};
		ImColor SelColor;

		std::vector<std::weak_ptr<Object>> selectedObjects_;
	};

	extern Hierarchy hierarchy_;
}

