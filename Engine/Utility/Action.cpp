#include "Action.h"
#include "Input.h"
#include <Engine/Dependencies/glfw-3.2.1/include/GLFW/glfw3.h>
#include "Engine/Graphics/Graphics.h"

namespace Sapphire
{
	ActionManager actionManager_;

	ActionManager::ActionManager()
	{
		LoadDefaultActions();
	}

	ActionManager::~ActionManager()
	{

	}

	const ActionStateTest &ActionManager::GetState(const std::string &passedAction)
	{
		return allActionStates[passedAction].CurrentState;
	}

	void ActionManager::LoadDefaultActions()
	{
		//[DR][NOTE] - Map && bind Actions
		allActionStates["MoveUp"] = Action(Key_W);
		allActionStates["MoveDown"] = Action(Key_S);
		allActionStates["MoveLeft"] = Action(Key_A);
		allActionStates["MoveRight"] = Action(Key_D);
		allActionStates["UnitSelect"] = Action(MouseButton_Left);
		allActionStates["UnitSelectAdd"] = Action(Key_Shift);
		allActionStates["UnitOrder"] = Action(MouseButton_Right);
		allActionStates["ToggleState"] = Action(Key_P);
		allActionStates["ToggleDebugUM"] = Action(Key_Q);
	}

	void ActionManager::Update()
	{
		UpdateActionState("MoveUp");
		UpdateActionState("MoveDown");
		UpdateActionState("MoveLeft");
		UpdateActionState("MoveRight");
		UpdateActionState("UnitSelect");
		UpdateActionState("UnitSelectAdd");
		UpdateActionState("UnitOrder");
		UpdateActionState("ToggleState");
		UpdateActionState("ToggleDebugUM");

		//ImVec2 winPos = ImGui::GetWindowPos();

		//cursorScreenPos.SetX(ImGui::GetMousePos().x);
		//cursorScreenPos.SetY(ImGui::GetMousePos().y);
		//cursorWorldPos.SetX((float(cursorScreenPos.GetX() - winPos.x - camPos.x) / tileSize) - 0.1875f * float(tileSize) /*6*/);
		//cursorWorldPos.SetZ(-(float(cursorScreenPos.GetY() - winPos.x - -camPos.y) / tileSize) - 0.0421875f * float(tileSize) /*1.35f*/);

		//cursorTilePos.SetX(float(cursorWorldPos.GetX()));
		//cursorTilePos.SetY(-float(cursorWorldPos.GetZ()));

		if (allActionStates["UnitSelect"].CurrentState == Initiated) 
		{
			int i = 0;
		}
	}

	void ActionManager::UpdateActionState(std::string const &passedAction)
	{
		if (inputManager_.AllKeyStates_[allActionStates[passedAction].BoundKey].isPressed)
		{
			allActionStates[passedAction].CurrentState = Initiated;
			return;
		}
		else if (inputManager_.AllKeyStates_[allActionStates[passedAction].BoundKey].isHeld)
		{
			allActionStates[passedAction].CurrentState = Active;
			return;
		}
		else if (inputManager_.AllKeyStates_[allActionStates[passedAction].BoundKey].isReleased)
		{
			allActionStates[passedAction].CurrentState = Exit;
			return;
		}
		else {
			allActionStates[passedAction].CurrentState = InActive;
			return;
		}
	}

	void ActionManager::BindAction(std::string act, char k)
	{
		allActionStates[act].BoundKey = k;
	}
}
