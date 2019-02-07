#include "SceneView.h"
#include "Engine/Graphics/Graphics.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Engine.h"
#include "Engine/Utility/Input.h"


namespace Sapphire
{
	SceneView sceneView_;

	SceneView::SceneView()
	{
		name_ = "Scene";
	}

	SceneView::~SceneView()
	{
	}

	void SceneView::Start()
	{
		width_ = graphics_.windowWidth_ * 3 / 5;
		height_ = graphics_.windowHeight_ * 3 / 4;
		positionX_ = graphics_.windowWidth_ / 5;
		positionY_ = 0;
		ImGui::SetNextWindowBgAlpha(0.0);
		InterfaceElement::Start();
	}

	void SceneView::Update()
	{
		ShowWorldOrigin();
		ShowGrid(1);
		ImGui::Text("Objects: [%i]", objectManager_.allObjects_.size());
		//ImGui::Text("Camera Pos x [%f]", graphics_.camera_.GetPositionWorld().x);
		//ImGui::Text("Camera Pos y [%f]", graphics_.camera_.GetPositionWorld().y);
		CameraMovement();
	}

	void SceneView::End()
	{
		InterfaceElement::End();
	}

	void SceneView::ShowWorldOrigin() const
	{
		const ImVec2& worldOriginScreen = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2),
			round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndX = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2 + engine_.GetTileSize()), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));
		const ImVec2 &worldOriginScreenAxisEndY = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2 - engine_.GetTileSize()));

		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndX, ImColor(255, 0, 0), 2);
		ImGui::GetWindowDrawList()->AddLine(worldOriginScreen, worldOriginScreenAxisEndY, ImColor(0, 255, 0), 2);
	}

	void SceneView::ShowGrid(const int& divisions) const
	{
		//const ImVec2 &worldOriginScreen = ImVec2(round(-graphics_.camera_.GetPositionWorld().x + graphics_.windowWidth_ / 2), round(graphics_.camera_.GetPositionWorld().y + graphics_.windowHeight_ / 2));

		int amountOfLines = 0;
		//vertical lines
		const int offsetFixHor = graphics_.windowWidth_ / 2 % engine_.GetTileSize();
		for (int horizontalLocation = -engine_.GetTileSize() - int(graphics_.camera_.GetPositionWorld().x) % engine_.GetTileSize();
			horizontalLocation < graphics_.windowWidth_ + engine_.GetTileSize() + int(graphics_.camera_.GetPositionWorld().x) % engine_.GetTileSize();
			horizontalLocation += engine_.GetTileSize())
		{
			amountOfLines++;
			const ImVec2 &LineTop = ImVec2(float(horizontalLocation + offsetFixHor), 0.f);
			const ImVec2 &LineBottom = ImVec2(float(horizontalLocation + offsetFixHor), float(graphics_.windowHeight_));
			ImGui::GetWindowDrawList()->AddLine(LineTop, LineBottom, ImColor(0, 0, 0, 64), 1);
		}

		//Horizontal lines
		const int offsetFixVert = graphics_.windowHeight_ / 2 % engine_.GetTileSize();
		for (int verticalLocation = -engine_.GetTileSize() - -int(graphics_.camera_.GetPositionWorld().y) % engine_.GetTileSize();
			verticalLocation < graphics_.windowHeight_ + engine_.GetTileSize() + -int(graphics_.camera_.GetPositionWorld().y) % engine_.GetTileSize();
			verticalLocation += engine_.GetTileSize())
		{
			amountOfLines++;
			const ImVec2 &LineStart = ImVec2(0.f, float(verticalLocation + offsetFixVert));
			const ImVec2 &LineEnd = ImVec2(float(graphics_.windowWidth_), float(verticalLocation + offsetFixVert));
			ImGui::GetWindowDrawList()->AddLine(LineStart, LineEnd, ImColor(0, 0, 0, 64), 1);
		}

		ImGui::Text("Grid Lines: [%i]", amountOfLines);
		ImGui::Text("ScreenHeight: [%i], Offset [%i]", graphics_.windowHeight_, graphics_.windowHeight_ / 2 % engine_.GetTileSize());
	}

	void SceneView::CameraMovement()
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
				graphics_.camera_.OffsetPosition(float(-inputManager_.cursorScreenDelta.x), float(inputManager_.cursorScreenDelta.y));
			}
			if (inputManager_.AllKeyStates_[Key_C].isPressed)
			{
				graphics_.camera_.SetPositition(0, 0);
			}
		}
	}
}
