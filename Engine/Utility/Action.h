#pragma once
#include <iostream>
#include <memory>
#include "Engine/Dependencies/glm/glm.hpp"
#include <map>

//#include "../Core/Commander.h"

enum ActionStateTest {
	InActive,
	Initiated,
	Active,
	Exit
};

struct Action
{
	Action() = default;
	Action(const char &n)
	{
		BoundKey = n;
	}
	ActionStateTest CurrentState = InActive;
	char BoundKey = NULL;
};
//[KR][NOTE] Storing all the possible Actions passed to the Commander
//struct AllActionStates 
//{
//	Action MoveUp,
//		MoveDown,
//		MoveLeft,
//		MoveRight,
//		RangeIncrease,
//		RangeDecrease,
//		AddUnits,
//		RemoveUnits,
//		DeselectAllUnits,
//		MoveUnitsToCommander,
//		ToggleFavorite; //not active atm
//};

namespace Sapphire
{
	class ActionManager
	{
	public:
		ActionManager();
		~ActionManager();

		//[KR][NOTE] Default Update
		void Update();
		
		//[KR][NOTE] Compare Actions with bound Keys and update their states

		void BindAction(std::string act, char k);

		const ActionStateTest &GetState(const std::string &passedAction);
		
	private:
		void LoadDefaultActions();
		void UpdateActionState(std::string const &passedAction);
		std::map<std::string, Action> allActionStates;

	};

	extern ActionManager actionManager_;
}