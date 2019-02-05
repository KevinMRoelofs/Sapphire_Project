#include "Outliner.h"

#include "Engine/Engine.h"
#include "Engine/Utility/Random.h"
#include "Engine/Resources/Actor.h"
#include "Engine/Resources/Player.h"
#include "Engine/Utility/Input.h"

#include "Editor/Dependencies/imgui-master/imgui.h"
#include "Engine/Graphics/Graphics.h"
#include "Editor/Dependencies/imgui-master/imgui_internal.h"

namespace Sapphire
{
	Outliner outliner_;

	Outliner::Outliner()
	{
	}


	Outliner::~Outliner()
	{
	}

	void Outliner::Display()
	{
	}

	void Outliner::Selection()
	{
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin("", (bool*)NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowSize(ImVec2(float(graphics_.windowWidth_ + engine_.GetTileSize()), float(graphics_.windowHeight_ + engine_.GetTileSize())));
		ImGui::SetWindowPos(ImVec2(float(-engine_.GetTileSize()) / 2, float(-engine_.GetTileSize()) / 2));
		ImColor SelColor = ImColor(0.f, 0.f, 1.f, 0.5f);

		
		cursorInScene = true;
		/*if (ImGui::IsWindowHovered())
		{
			printf("%i \n", random_.GenerateInt(1, 100));
		}
		else
		{
			cursorInScene = false;
		}*/
		ShowGrid();
		ShowWorldOrigin();


		cursorPosLastframe = cursorPosCurrent;
		cursorPosCurrent = glm::vec2(inputManager_.cursorPosScreen.x, inputManager_.cursorPosScreen.y);

		//Select All Objects with CTRL + A
		if (inputManager_.AllKeyStates_[Key_Control].isHeld && inputManager_.AllKeyStates_[Key_A].isPressed)
		{
			for (auto &object : objectManager_.allObjects_)
			{
				Select(object);
			}
		}

		if (inputManager_.AllKeyStates_[MouseButton_Left].isPressed)
		{
			cursorPosStart = cursorPosCurrent;
		}

		//delete all object in selection when pressing DELETE KEY
		if (inputManager_.AllKeyStates_[Key_Delete].isPressed)
		{
			while(!selectedObjects_.empty())
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

		for (auto &object : selectedObjects_)
		{
			//Enable Dragging if a Select Object is clicked on
			if (inputManager_.AllKeyStates_[MouseButton_Left].isPressed && abs(object.lock()->GetPositionScreen().x - cursorPosCurrent.x) < object.lock()->GetScale().x / 2 * engine_.GetTileSize() &&
				abs(object.lock()->GetPositionScreen().y - cursorPosCurrent.y) < object.lock()->GetScale().y / 2 * engine_.GetTileSize())
			{
				cursorDraggingSelObject = true;
			}
			//Add an outline to all Selected Objects
			ImGui::GetWindowDrawList()->AddRect(ImVec2(round(object.lock()->GetPositionScreen().x + object.lock()->GetScale().x * engine_.GetTileSize() / 2.f),
				round(object.lock()->GetPositionScreen().y + object.lock()->GetScale().y * engine_.GetTileSize() / 2.f)),
				ImVec2(round(object.lock()->GetPositionScreen().x - object.lock()->GetScale().x * engine_.GetTileSize() / 2.f),
					round(object.lock()->GetPositionScreen().y - object.lock()->GetScale().y * engine_.GetTileSize() / 2.f)), SelColor);
			int i = 0;
		}
		if (cursorDraggingSelObject)
		{
			//Set new positions to Dragged Objects
			for (auto &object : selectedObjects_)
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

				int i = 0;
			}
		}
		//ImGui::End();
		
		ImGui::Begin("Object Outliner",(bool*)NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(float(graphics_.windowWidth_ / 5), float(graphics_.windowHeight_)));
		ImGui::SetWindowPos(ImVec2(0, 0));
		{
			//Spawn Nico on Press and Release
			if(ImGui::ImageButton((void*)(intptr_t)textureManager_.GetTexture("../Assets/Player/Nico.png").lock()->textureId_, ImVec2(64, 64)))
			{
				glm::vec3 newPos = glm::vec3(0);
				newPos.x = random_.GenerateFloat(-10.f, 10.f);
				newPos.y = random_.GenerateFloat(5.5f, 9.5f);

				actorManager_.CreateActor("../Assets/Player/Nico.png", newPos, nullptr);
				//cursorInMenu = true;
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			int currentSelectedObject = 0;
			for (auto &object : selectedObjects_)
			{

				std::weak_ptr<Actor> actorPtr = std::dynamic_pointer_cast<Actor>(object.lock());
				std::weak_ptr<Player> playerPtr = std::dynamic_pointer_cast<Player>(actorPtr.lock());
				ImGui::PushID(object.lock()->GetId());
				ImGui::Text("Nico[%i] at: ", object.lock()->GetId());
				glm::vec2 actorPos;
				actorPos.x = object.lock()->GetPositionWorld().x;
				actorPos.y = object.lock()->GetPositionWorld().y;
				ImGui::SameLine();
				ImGui::SliderFloat2("", reinterpret_cast<float*>(&actorPos), -10, 10);
				object.lock()->SetPositionWorld(actorPos.x, actorPos.y);



				//ImGui::Text("ObjectPosScreen x: %.0f, y: %.0f", object.lock()->GetPositionScreen().x, object.lock()->GetPositionScreen().y);

				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					if (selectedObjects_.size() > 1) std::rotate(selectedObjects_.begin(), selectedObjects_.begin() + currentSelectedObject + 1, selectedObjects_.end());
					selectedObjects_.pop_back();
					if (selectedObjects_.size() > 1) std::rotate(selectedObjects_.begin(), selectedObjects_.end() - currentSelectedObject, selectedObjects_.end());
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
					//printf("Actors: %i - Objects: %i \n", static_cast<int>(actorManager_.allActors_.size()), static_cast<int>(objectManager_.allObjects_.size()));
					ImGui::PopID();
					break;
				}
				if (!playerPtr.expired())
				{
					ImGui::SameLine();
					ImGui::Text("Is Player[%i]", playerPtr.lock()->playerId_);

					//ImGui::Text("PlayerScreenPos x: %.0f, y: %.0f", playerPtr.lock()->GetPositionScreen().x, playerPtr.lock()->GetPositionScreen().y);
				}
				ImGui::PopID();
				currentSelectedObject++;
			}
			ImGui::Text("MousePos x: %.0f, y: %.0f", inputManager_.cursorPosScreen.x, inputManager_.cursorPosScreen.y);
			ImGui::Text("MouseOffset x: %.0f, y: %.0f", inputManager_.cursorPosScreen.x - cursorPosLastframe.x, inputManager_.cursorPosScreen.y - cursorPosLastframe.y);
			ImGui::Text("MousePosWorld x: %.0f, y: %.0f", inputManager_.cursorPosWorld.x, inputManager_.cursorPosWorld.y);
			ImGui::Text("CameraPosWorld x: %.0f, y: %.0f", graphics_.camera_.GetPositionWorld().x, graphics_.camera_.GetPositionWorld().y);
		}
		ImGui::End();

		//Selection Box
		if (inputManager_.AllKeyStates_[MouseButton_Left].isHeld && cursorInScene && !cursorDraggingSelObject && glm::distance(cursorPosStart, cursorPosCurrent) > 4)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPosCurrent.x, cursorPosCurrent.y), ImVec2(cursorPosStart.x, cursorPosStart.y), SelColor);
			if (cursorPosCurrent != cursorPosLastframe)
			{
				if (!inputManager_.AllKeyStates_[Key_Shift].isHeld) selectedObjects_.clear();
				for (auto &object : objectManager_.allObjects_)
				{
					if ((object->GetPositionScreen().x > cursorPosStart.x && object->GetPositionScreen().x < cursorPosCurrent.x || object->GetPositionScreen().x < cursorPosStart.x && object->GetPositionScreen().x > cursorPosCurrent.x) &&
						(object->GetPositionScreen().y > cursorPosStart.y && object->GetPositionScreen().y < cursorPosCurrent.y || object->GetPositionScreen().y < cursorPosStart.y && object->GetPositionScreen().y > cursorPosCurrent.y))
					{
						Select(object);
					}
				}
			}
		}


		if (inputManager_.AllKeyStates_[MouseButton_Left].isReleased)
		{
			//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(cursorPosCurrent.x, cursorPosCurrent.y), ImVec2(cursorPosStart.x, cursorPosStart.y), SelColor);
			if (!inputManager_.AllKeyStates_[Key_Shift].isHeld && glm::distance(cursorPosStart, cursorPosCurrent) < 4)
			{
				if (!cursorInScene && !cursorDraggingSelObject) {
					selectedObjects_.clear();
					//printf("List Cleared \n");
				}
			}

			cursorDraggingSelObject = false;

			for (auto &object : objectManager_.allObjects_)
			{
				if (glm::distance(cursorPosStart, cursorPosCurrent) < 4 && abs(object->GetPositionScreen().x - cursorPosCurrent.x) < object->GetScale().x / 2 * engine_.GetTileSize() &&
					abs(object->GetPositionScreen().y - cursorPosCurrent.y) < object->GetScale().y / 2 * engine_.GetTileSize())
				{
					Select(object);
				}
			}
		}
		ImGui::End();
	}

	bool Outliner::Select(std::weak_ptr<Object> selectedObject)
	{
		bool objectAlreadyInSelection = false;
		for (auto &selObject : selectedObjects_)
		{
			if (selObject.lock() == selectedObject.lock())
			{
				objectAlreadyInSelection = true;
				break;
			}
		}
		if (!objectAlreadyInSelection)
		{
			selectedObjects_.push_back(selectedObject.lock());
			return true;
		}
		return false;
	}

	void Outliner::Deselect(std::weak_ptr<Object> deselectedObject)
	{

	}

	void Outliner::ShowWorldOrigin()
	{
		const ImVec2& worldOriginScreen = ImVec2(round(graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2),
			round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndX = ImVec2(round(graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2 + engine_.GetTileSize()), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndY = ImVec2(round(graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2 - engine_.GetTileSize()));

		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndX, ImColor(255, 0, 0), 2);
		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndY, ImColor(0, 255, 0), 2);
	}

	void Outliner::ShowGrid(const int& divisions)
	{
		const ImVec2 &worldOriginScreen = ImVec2(round(graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));

		//vertical lines
		for (int horizontalLocation = 0;
			horizontalLocation < graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_;
			horizontalLocation += engine_.GetTileSize())
		{
			const ImVec2 &LineTop = ImVec2(float(horizontalLocation), 0.f);
			const ImVec2 &LineBottom = ImVec2(float(horizontalLocation), float(graphics_.windowHeight_));
			ImGui::GetWindowDrawList()->AddLine(LineBottom, LineTop, ImColor(0, 0, 0, 127), 1);
		}
	}

	const glm::vec2& Outliner::GetCursorScreen()
	{
		return cursorPosCurrent;
	}

	const glm::vec2& Outliner::GetCursorScreenLastFrame()
	{
		return cursorPosLastframe;
	}
}
