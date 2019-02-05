#pragma once
#include <vector>
#include "Engine/Resources/Object.h"
#include "Editor/Dependencies/imgui-master/imgui.h"

namespace Sapphire
{
	class Outliner
	{
	public:
		Outliner();
		~Outliner();

		void Display();

		const glm::vec2 &GetCursorScreen();
		const glm::vec2 &GetCursorScreenLastFrame();

		void Selection();
	private:

		bool Select(std::weak_ptr<Object> selectedObject);
		void Deselect(std::weak_ptr<Object> deselectedObject);

		void ShowWorldOrigin();
		void ShowGrid(const int &divisions = 0);


		glm::vec2 cursorPosStart, cursorPosCurrent, cursorPosLastframe;

		bool cursorDraggingNewObject = false;
		bool cursorInScene = false;
		bool cursorDraggingSelObject = false;

		std::vector<std::weak_ptr<Object>> selectedObjects_;
	};

	extern Outliner outliner_;
}
