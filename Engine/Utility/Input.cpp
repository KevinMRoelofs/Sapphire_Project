#include "Input.h"
//#include <cassert>

#include <algorithm>
#include "Engine/Graphics/Graphics.h"
#include <Editor/Dependencies/imgui-master/imgui.h>
#include "Engine/Engine.h"

using namespace Sapphire;

namespace Sapphire
{
	InputManager inputManager_;

	InputManager::InputManager()
	{
		CheckConnectionGamePads();
	}

	InputManager::~InputManager()
	{
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			VibrateGamePad(i, 0, 0);
		}
	}



	void InputManager::CheckConnectionGamePads()
	{
		int controllerId = -1;

		for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
		{
			ZeroMemory(&GamePads_[i].XinputState, sizeof(XINPUT_STATE));

			if (XInputGetState(i, &GamePads_[i].XinputState) == ERROR_SUCCESS) {

				GamePads_[i].isConnected = true;
				controllerId = i;
			}
			else {
				//[KR][NOTE] Resets the GamePad on disconnect
				GamePad blankGamePad;
				GamePads_[i] = blankGamePad;
				GamePads_[i].isConnected = false;

			}
		}
	}

	void InputManager::UpdateInputGamePads()
	{
		CheckConnectionGamePads();

		if (GetFocus() != NULL) {
			for (DWORD i = 0; i < XUSER_MAX_COUNT && GamePads_[i].isConnected; i++)
			{
				GamePad &currentGamePad = GamePads_[i];
				WORD &currentButtons = currentGamePad.XinputState.Gamepad.wButtons;
				UpdateGamePadButtonKeyState(currentGamePad.A, currentButtons & XINPUT_GAMEPAD_A);
				UpdateGamePadButtonKeyState(currentGamePad.B, currentButtons & XINPUT_GAMEPAD_B);
				UpdateGamePadButtonKeyState(currentGamePad.X, currentButtons & XINPUT_GAMEPAD_X);
				UpdateGamePadButtonKeyState(currentGamePad.Y, currentButtons & XINPUT_GAMEPAD_Y);
				UpdateGamePadButtonKeyState(currentGamePad.DPADUP, currentButtons & XINPUT_GAMEPAD_DPAD_UP);
				UpdateGamePadButtonKeyState(currentGamePad.DPADDOWN, currentButtons & XINPUT_GAMEPAD_DPAD_DOWN);
				UpdateGamePadButtonKeyState(currentGamePad.DPADRIGHT, currentButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
				UpdateGamePadButtonKeyState(currentGamePad.DPADLEFT, currentButtons & XINPUT_GAMEPAD_DPAD_LEFT);
				UpdateGamePadButtonKeyState(currentGamePad.START, currentButtons & XINPUT_GAMEPAD_START);
				UpdateGamePadButtonKeyState(currentGamePad.BACK, currentButtons & XINPUT_GAMEPAD_BACK);
				UpdateGamePadButtonKeyState(currentGamePad.LB, currentButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
				UpdateGamePadButtonKeyState(currentGamePad.RB, currentButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
				UpdateGamePadButtonKeyState(currentGamePad.L3, currentButtons & XINPUT_GAMEPAD_LEFT_THUMB);
				UpdateGamePadButtonKeyState(currentGamePad.R3, currentButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

				float normLX = fmaxf(-1, (float)currentGamePad.XinputState.Gamepad.sThumbLX / 32767);
				currentGamePad.LEFTSTICK.x = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
				float normLY = fmaxf(-1, (float)currentGamePad.XinputState.Gamepad.sThumbLY / 32767);
				currentGamePad.LEFTSTICK.y = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));
				if (deadzoneX > 0) currentGamePad.LEFTSTICK.x *= 1 / (1 - deadzoneX);
				if (deadzoneY > 0) currentGamePad.LEFTSTICK.y *= 1 / (1 - deadzoneY);

				float normRX = fmaxf(-1, (float)currentGamePad.XinputState.Gamepad.sThumbRX / 32767);
				currentGamePad.RIGHTSTICK.x = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
				float normRY = fmaxf(-1, (float)currentGamePad.XinputState.Gamepad.sThumbRY / 32767);
				currentGamePad.RIGHTSTICK.y = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));
				if (deadzoneX > 0) currentGamePad.RIGHTSTICK.x *= 1 / (1 - deadzoneX);
				if (deadzoneY > 0) currentGamePad.RIGHTSTICK.y *= 1 / (1 - deadzoneY);

				currentGamePad.LEFTTRIGGER = (float)currentGamePad.XinputState.Gamepad.bLeftTrigger / 255;
				currentGamePad.RIGHTTRIGGER = (float)currentGamePad.XinputState.Gamepad.bRightTrigger / 255;


			}
		}
		else {
			for (DWORD i = 0; i < XUSER_MAX_COUNT && GamePads_[i].isConnected; i++)
			{
				GamePad blankGamePad;
				GamePad &currentGamePad = GamePads_[i];
				currentGamePad = blankGamePad;
			}

		}



		/*POINT MouseCursor;
		GetCursorPos(&MouseCursor);

		MouseCursor.x += round(GamePads[0].LEFTSTICK.x * 5);
		MouseCursor.y += round(GamePads[0].LEFTSTICK.y * 5);

		SetCursorPos(MouseCursor.x, MouseCursor.y);*/
	}

	void InputManager::UpdateInputKeyboardMouse()
	{
		if (GetFocus() != NULL) {

			cursorScreenLastPos = cursorPosScreen;
			glfwGetCursorPos(graphics_.window_, &cursorPosScreen.x, &cursorPosScreen.y);
			cursorScreenDelta = cursorScreenLastPos - cursorPosScreen;
			cursorPosWorld = CursorPos(cursorPosScreen.x - graphics_.camera_.GetPositionWorld().x - float(graphics_.windowWidth_) / 2, cursorPosScreen.y - graphics_.camera_.GetPositionWorld().y - float(graphics_.windowHeight_) / 2);

			for (unsigned int i = 0; i < allKeyValues.size(); i++) {
				KeyState &currentKey = AllKeyStates_[allKeyValues[i]];
				if (GetKeyState(allKeyValues[i]) & 0x8000) {			//[KR][NOTE] detect if the key is pressed
					if (currentKey.isHeld) continue;					//[KR][NOTE] check if the key is already held, if true check next key
					else if (currentKey.isPressed) {					//[KR][NOTE] check if the key was already pressed last frame
						currentKey.isPressed = false;					//[KR][NOTE] if the key was already pressed, set key to held and remove isPressed state
						currentKey.isHeld = true;
						continue;
					}
					else {												//[KR][NOTE] if the key was not already pressed, set state to isPressed
						currentKey.isPressed = true;
						currentKey.isHeld = false;
						currentKey.isReleased = false;
						continue;
					}
				}
				else if (currentKey.isPressed || currentKey.isHeld) {	//[KR][NOTE] if the key was pressed or held last frame, set key to isReleased
					currentKey.isPressed = false;
					currentKey.isHeld = false;
					currentKey.isReleased = true;
					continue;
				}
				else if (currentKey.isReleased) {						//[KR][NOTE] if the key was released last frame, set isReleased to false
					currentKey.isReleased = false;
					continue;
				}
			}
		}
		else {	//[KR][NOTE] Resets all Keystate when leaving Window
			for (unsigned int i = 0; i < allKeyValues.size(); i++) {
				KeyState &currentKey = AllKeyStates_[allKeyValues[i]];
				currentKey.isPressed = false;
				currentKey.isHeld = false;
				currentKey.isReleased = false;
			}
		}
	}

	void InputManager::PrintActiveKeys()
	{
		//[KR][NOTE] Prints if the key is Pressed, Held or Released
		for (unsigned int i = 0; i < allKeyValues.size(); i++) {
			KeyState &currentKey = AllKeyStates_[allKeyValues[i]];
			if (currentKey.isPressed) std::cout << char(allKeyValues[i]) << " value: " << allKeyValues[i] << " is Pressed" << std::endl;
			if (currentKey.isHeld) std::cout << char(allKeyValues[i]) << " value: " << allKeyValues[i] << " is Held" << std::endl;
			if (currentKey.isReleased) std::cout << char(allKeyValues[i]) << " value: " << allKeyValues[i] << " is Released" << std::endl;
		}
	}

	InputManager::CursorPos InputManager::CursorPos::operator-(CursorPos cp)
	{
		return CursorPos(cp.x - x, cp.y - y);
	}

	void InputManager::UpdateGamePadButtonKeyState(KeyState & Button, bool ButtonIsActive)
	{
		if (ButtonIsActive) {
			if (Button.isHeld) return;			//[KR][NOTE] check if the key is already held, if true check next key
			else if (Button.isPressed) {			//[KR][NOTE] check if the key was already pressed last frame
				Button.isPressed = false;        //[KR][NOTE] if the key was already pressed, set key to held and remove isPressed state
				Button.isHeld = true;
				return;
			}
			else {														//[KR][NOTE] if the key was not already pressed, set state to isPressed
				Button.isPressed = true;
				Button.isHeld = false;
				Button.isReleased = false;
				return;
			}
		}
		else if (Button.isPressed || Button.isHeld) { //[KR][NOTE] if the key was pressed or held last frame, set key to isReleased
			Button.isPressed = false;
			Button.isHeld = false;
			Button.isReleased = true;
			return;
		}
		else if (Button.isReleased) {			//[KR][NOTE] if the key was released last frame, set isReleased to false
			Button.isReleased = false;
			return;
		}
	}

	void InputManager::VibrateGamePad(int port, float leftMotor, float rightMotor)
	{

		XINPUT_VIBRATION Vibration;

		memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

		int leftVib = (int)(leftMotor*65535.0f);
		int rightVib = (int)(rightMotor*65535.0f);

		// Set the Vibration Values
		Vibration.wLeftMotorSpeed = leftVib;
		Vibration.wRightMotorSpeed = rightVib;
		// Vibrate the controller
		XInputSetState((int)port, &Vibration);

	}

	void InputManager::PrintActiveGamePads()
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++) {
			GamePad const &currentGamePad = GamePads_[i];
			if (currentGamePad.A.isPressed) std::cout << "GamePad: " << i << " | Button: A is Pressed" << std::endl;
			if (currentGamePad.A.isHeld) std::cout << "GamePad: " << i << " | Button: A is Held" << std::endl;
			if (currentGamePad.A.isReleased) std::cout << "GamePad: " << i << " | Button: A is Released" << std::endl;
			if (GamePads_[i].LEFTSTICK.x != 0)std::cout << "GamePad: " << i << " LeftX: " << GamePads_[i].LEFTSTICK.x << std::endl;
			if (GamePads_[i].LEFTSTICK.y != 0)std::cout << "GamePad: " << i << " LeftY: " << GamePads_[i].LEFTSTICK.y << std::endl;
			if (GamePads_[i].RIGHTSTICK.x != 0)std::cout << "GamePad: " << i << " RightX: " << GamePads_[i].RIGHTSTICK.x << std::endl;
			if (GamePads_[i].RIGHTSTICK.y != 0)std::cout << "GamePad: " << i << " RightY: " << GamePads_[i].RIGHTSTICK.y << std::endl;
			if (GamePads_[i].RIGHTTRIGGER != 0)std::cout << "GamePad: " << i << " RightTrigger: " << GamePads_[i].RIGHTTRIGGER << std::endl;
			if (GamePads_[i].LEFTTRIGGER != 0)std::cout << "GamePad: " << i << " LeftTrigger: " << GamePads_[i].LEFTTRIGGER << std::endl;

		}
	}
}
