#include "Hierarchy.h"
#include "Engine/Graphics/Graphics.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "SceneView.h"
#include "Engine/Utility/Input.h"
#include "Engine/Resources/Actor.h"
#include "Engine/Engine.h"
#include "Engine/Resources/Player.h"

namespace Sapphire
{
	Hierarchy hierarchy_;

	Hierarchy::Hierarchy()
	{
		name_ = "Hierarchy";
		positionX_ = 0;
		positionY_ = 0;
		cursorDraggingSelObject = false;
	}


	Hierarchy::~Hierarchy()
	{
	}

	void Hierarchy::Start()
	{
		width_ = graphics_.windowWidth_ * 1 / 5;
		height_ = graphics_.windowHeight_;
		SelColor = ImColor(0.f, 0.f, 1.f, 0.5f);
		InterfaceElement::Start();
	}

	void Hierarchy::Update()
	{
		//ImGui::Text("Hierarchy");
		for (auto &object : objectManager_.allObjects_)
		{
			//Use lines below later for filters
			//std::weak_ptr<Actor> actorPtr = std::dynamic_pointer_cast<Actor>(object.lock());
			//std::weak_ptr<Player> playerPtr = std::dynamic_pointer_cast<Player>(actorPtr.lock());
			ImGui::PushID(object->GetId());
			//bool selectState = false;
			char buf[32];
			sprintf_s(buf, "Object %i", object->GetId());
			if (ImGui::Selectable(buf, IsInSelection(object)))
			{
				if (inputManager_.AllKeyStates_[Key_Control].isHeld) Deselect(object);
				else Select(object, inputManager_.AllKeyStates_[Key_Shift].isHeld);
			}
			//ImGui::Text("[%i] - X[%.2f], Y[%.2f]", object->GetId(), object->GetPositionWorld().x, object->GetPositionWorld().y);
			ImGui::PopID();
		}

		Selecting();
		SelectionBox();
		SelectionActions();
	}

	void Hierarchy::End()
	{

		InterfaceElement::End();
	}

	void Hierarchy::Selecting()
	{
		if (sceneView_.cursorHover)
		{
			cursorPosLastframe = cursorPosCurrent;
			cursorPosCurrent = glm::vec2(inputManager_.cursorPosScreen.x, inputManager_.cursorPosScreen.y);

			//Select All Objects with CTRL + A
			if (inputManager_.AllKeyStates_[Key_Control].isHeld && inputManager_.AllKeyStates_[Key_A].isPressed)
			{
				for (auto &object : objectManager_.allObjects_)
				{
					Select(object, true);
				}
			}

			if (inputManager_.AllKeyStates_[MouseButton_Left].isPressed)
			{
				cursorPosStart = cursorPosCurrent;
			}




			if (inputManager_.AllKeyStates_[MouseButton_Left].isReleased)
			{
				//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(cursorPosCurrent.x, cursorPosCurrent.y), ImVec2(cursorPosStart.x, cursorPosStart.y), SelColor);
				if (!inputManager_.AllKeyStates_[Key_Shift].isHeld && glm::distance(cursorPosStart, cursorPosCurrent) < 4)
				{
					if (!cursorDraggingSelObject) {
						selectedObjects_.clear();
						//printf("List Cleared \n");
					}
				}

				cursorDraggingSelObject = false;

				//Selecting an object
				for (auto &object : objectManager_.allObjects_)
				{
					if (glm::distance(cursorPosStart, cursorPosCurrent) < 4
						&& cursorPosCurrent.x > object->GetPositionScreen().x + object->GetMesh()->GetBoundsMin().x * engine_.GetTileSize()
						&& cursorPosCurrent.x < object->GetPositionScreen().x + object->GetMesh()->GetBoundsMax().x * engine_.GetTileSize()
						&& cursorPosCurrent.y < object->GetPositionScreen().y - object->GetMesh()->GetBoundsMin().y * engine_.GetTileSize()
						&& cursorPosCurrent.y > object->GetPositionScreen().y - object->GetMesh()->GetBoundsMax().y * engine_.GetTileSize())
					{
						Select(object, inputManager_.AllKeyStates_[Key_Shift].isHeld);
						if (inputManager_.AllKeyStates_[Key_Control].isHeld) Deselect(object);
					}
				}
			}
		}
	}

	void Hierarchy::SelectionActions()
	{
		for (auto &object : selectedObjects_)
		{
			//Enable Dragging if a Select Object is clicked on
			if (inputManager_.AllKeyStates_[MouseButton_Left].isPressed && cursorPosCurrent.x > object.lock()->GetPositionScreen().x + object.lock()->GetMesh()->GetBoundsMin().x * engine_.GetTileSize() && cursorPosCurrent.x < object.lock()->GetPositionScreen().x + object.lock()->GetMesh()->GetBoundsMax().x * engine_.GetTileSize() &&
				cursorPosCurrent.y < object.lock()->GetPositionScreen().y - object.lock()->GetMesh()->GetBoundsMin().y * engine_.GetTileSize() && cursorPosCurrent.y > object.lock()->GetPositionScreen().y - object.lock()->GetMesh()->GetBoundsMax().y * engine_.GetTileSize())
			{
				cursorDraggingSelObject = true;
			}
			//Add an outline to all Selected Objects
			auto mesh = object.lock()->GetMesh();
			auto &startCorner = ImVec2(object.lock()->GetPositionScreen().x + object.lock()->GetMesh()->GetBoundsMin().x * engine_.GetTileSize(),
				object.lock()->GetPositionScreen().y - object.lock()->GetMesh()->GetBoundsMin().y * engine_.GetTileSize());
			auto &endCorner = ImVec2(object.lock()->GetPositionScreen().x + object.lock()->GetMesh()->GetBoundsMax().x * engine_.GetTileSize() + 1,
				object.lock()->GetPositionScreen().y - object.lock()->GetMesh()->GetBoundsMax().y * engine_.GetTileSize() + 1);
			sceneView_.drawlist_->AddRect(startCorner, endCorner, SelColor);

			//Set new positions to Dragged Objects
			if (cursorDraggingSelObject && sceneView_.cursorHover)
			{
				glm::vec2 newPos = object.lock()->GetPositionScreen();
				newPos.x += cursorPosCurrent.x - cursorPosLastframe.x - float(graphics_.windowWidth_) / 2;
				newPos.y += cursorPosCurrent.y - cursorPosLastframe.y - float(graphics_.windowHeight_) / 2;
				object.lock()->SetPositionScreen(newPos);
				std::weak_ptr<Actor> actorPtr = std::dynamic_pointer_cast<Actor>(object.lock());
				if (!actorPtr.expired())
				{
					actorPtr.lock()->velocity_ = glm::vec2(0);
				}
			}
		}
		//delete all object in selection when pressing DELETE KEY
		if (inputManager_.AllKeyStates_[Key_Delete].isPressed && (sceneView_.cursorHover || cursorHover))
		{
			while (!selectedObjects_.empty())
			{
				auto &object = selectedObjects_.back();
				std::weak_ptr<Actor> actorPtr = std::dynamic_pointer_cast<Actor>(object.lock());
				std::weak_ptr<Player> playerPtr = std::dynamic_pointer_cast<Player>(actorPtr.lock());

				if (!playerPtr.expired())
				{
					playerManager_.DeletePlayer(playerPtr);
				}
				else if (!actorPtr.expired())
				{
					actorManager_.DeleteActor(actorPtr);
				}
				else
				{
					objectManager_.DestroyObject(object);
				}
				selectedObjects_.pop_back();
			}
		}
	}

	void Hierarchy::SelectionBox()
	{
		//Selection Box
		if (inputManager_.AllKeyStates_[MouseButton_Left].isHeld && sceneView_.cursorHover && !cursorDraggingSelObject && glm::distance(cursorPosStart, cursorPosCurrent) > 4)
		{
			sceneView_.drawlist_->AddRectFilled(ImVec2(cursorPosCurrent.x, cursorPosCurrent.y), ImVec2(cursorPosStart.x, cursorPosStart.y), SelColor);
			if (cursorPosCurrent != cursorPosLastframe)
			{
				if (!inputManager_.AllKeyStates_[Key_Shift].isHeld) selectedObjects_.clear();
				for (auto &object : objectManager_.allObjects_)
				{
					if ((object->GetPositionScreen().x > cursorPosStart.x && object->GetPositionScreen().x < cursorPosCurrent.x || object->GetPositionScreen().x < cursorPosStart.x && object->GetPositionScreen().x > cursorPosCurrent.x)
						&& (object->GetPositionScreen().y > cursorPosStart.y && object->GetPositionScreen().y < cursorPosCurrent.y || object->GetPositionScreen().y < cursorPosStart.y && object->GetPositionScreen().y > cursorPosCurrent.y))
					{
						Select(object, true);
					}
				}
			}
		}
	}

	bool Hierarchy::Select(std::weak_ptr<Object> selectedObject, bool addToSelection)
	{
		if (!addToSelection) selectedObjects_.clear();
		if (!IsInSelection(selectedObject))
		{
			selectedObjects_.push_back(selectedObject.lock());
			return true;
		}
		return false;
	}

	void Hierarchy::Deselect(std::weak_ptr<Object> deselectedObject)
	{
		int currentElementIndex = 0;
		for (auto &object : selectedObjects_)
		{
			if (object.lock() == deselectedObject.lock())
			{
				//rotate selection to set deselected object to the back, pop_back, rotate back
				std::rotate(selectedObjects_.begin(), selectedObjects_.begin() + currentElementIndex + 1, selectedObjects_.end());
				selectedObjects_.pop_back();
				std::rotate(selectedObjects_.begin(), selectedObjects_.end() - currentElementIndex, selectedObjects_.end());
				return;
			}
			currentElementIndex++;
		}
	}

	bool Hierarchy::IsInSelection(std::weak_ptr<Object> passedObject)
	{
		for (auto &object : selectedObjects_)
		{
			if (object.lock() == passedObject.lock())
			{
				return true;
			}
		}
		return false;
	}
}
