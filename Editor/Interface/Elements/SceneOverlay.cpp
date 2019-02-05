#include "SceneOverlay.h"
#include "Engine/Graphics/Graphics.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Engine.h"
#include "Engine/Utility/Input.h"


namespace Sapphire
{
	SceneOverlay sceneOverlay_;

	SceneOverlay::SceneOverlay()
	{
		name_ = "Scene";
	}

	SceneOverlay::~SceneOverlay()
	{
	}

	void SceneOverlay::Start()
	{
		width_ = graphics_.windowWidth_ * 3 / 5;
		height_ = graphics_.windowHeight_ * 3 / 4;
		positionX_ = graphics_.windowWidth_ / 5;
		positionY_ = 0;
		ImGui::SetNextWindowBgAlpha(0.0);
		InterfaceElement::Start();
	}

	void SceneOverlay::Update()
	{
		ShowWorldOrigin();
		ShowGrid(1);
		ImGui::Text("Objects: [%i]", objectManager_.allObjects_.size());
		ImGui::Text("Camera Pos x [%f]", graphics_.camera_.GetPositionWorld().x);
		ImGui::Text("Camera Pos y [%f]", graphics_.camera_.GetPositionWorld().y);
		CameraMovement();
	}

	void SceneOverlay::End()
	{
		InterfaceElement::End();
	}

	void SceneOverlay::ShowWorldOrigin() const
	{
		const ImVec2& worldOriginScreen = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2),
			round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndX = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2 + engine_.GetTileSize()), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndY = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2 - engine_.GetTileSize()));

		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndX, ImColor(255, 0, 0), 2);
		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndY, ImColor(0, 255, 0), 2);
	}

	void SceneOverlay::ShowGrid(const int& divisions) const
	{
		//const ImVec2 &worldOriginScreen = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));

		//vertical lines
		int seven = 25 % 18;
		//const int cameraPosX = int(graphics_.camera_.GetPositionWorld().x) ? int(graphics_.camera_.GetPositionWorld().x) : engine_.GetTileSize();
		int amountOfLines = 0;
		for (int horizontalLocation = -engine_.GetTileSize() - int(graphics_.camera_.GetPositionWorld().x) % engine_.GetTileSize();
			horizontalLocation < graphics_.windowWidth_ + engine_.GetTileSize() + int(graphics_.camera_.GetPositionWorld().x) % engine_.GetTileSize();
			horizontalLocation += engine_.GetTileSize())
		{
			amountOfLines++;
			const ImVec2 &LineTop = ImVec2(float(horizontalLocation), 0.f);
			const ImVec2 &LineBottom = ImVec2(float(horizontalLocation), float(graphics_.windowHeight_));
			ImGui::GetWindowDrawList()->AddLine(LineBottom, LineTop, ImColor(0, 0, 0, 64), 1);
		}

		//Horizontal lines
		for (int verticalLocation = -engine_.GetTileSize() - -int(graphics_.camera_.GetPositionWorld().y) % engine_.GetTileSize();
			verticalLocation < graphics_.windowHeight_ + engine_.GetTileSize() + -int(graphics_.camera_.GetPositionWorld().y) % engine_.GetTileSize();
			verticalLocation += engine_.GetTileSize())
		{
			amountOfLines++;
			const ImVec2 &LineTop = ImVec2(0.f, float(verticalLocation));
			const ImVec2 &LineBottom = ImVec2(float(graphics_.windowWidth_), float(verticalLocation));
			ImGui::GetWindowDrawList()->AddLine(LineBottom, LineTop, ImColor(0, 0, 0, 64), 1);
		}

		ImGui::Text("Grid Lines: [%i]", amountOfLines);
	}

	void SceneOverlay::CameraMovement()
	{
		if (cursorHover)
		{
			if (!inputManager_.AllKeyStates_[Key_Control].isHeld)
			{
				if (inputManager_.AllKeyStates_[Key_A].isHeld)
				{
					graphics_.camera_.OffsetPosition(-0.5f, 0);
				}
				if (inputManager_.AllKeyStates_[Key_D].isHeld)
				{
					graphics_.camera_.OffsetPosition(0.5f, 0);
				}
				if (inputManager_.AllKeyStates_[Key_W].isHeld)
				{
					graphics_.camera_.OffsetPosition(0, 0.5f);
				}
				if (inputManager_.AllKeyStates_[Key_S].isHeld)
				{
					graphics_.camera_.OffsetPosition(0, -0.5f);
				}
			}
			if (inputManager_.AllKeyStates_[MouseButton_Right].isHeld)
			{
				//const glm::vec2 &cursorMovement = outliner_.GetCursorScreen() inputManager_.cursorPosScreen - outliner_.GetCursorScreenLastFrame();
				graphics_.camera_.OffsetPosition(-inputManager_.cursorScreenDelta.x, inputManager_.cursorScreenDelta.y);
			}
			if (inputManager_.AllKeyStates_[Key_C].isPressed)
			{
				graphics_.camera_.SetPositition(0, 0);
			}
		}
	}
}
