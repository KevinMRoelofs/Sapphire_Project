#pragma once
#pragma comment (lib, "XInput.lib")
#include <windows.h>
#include <map>
#include <iostream>
#include <array>
#include <Xinput.h>
#include <DirectXMath.h>
#include <assert.h>

//[KR][NOTE] Here are all the defines for the Key_ values are stored
#pragma region Keys
#define Key_A 'A'
#define Key_B 'B'
#define Key_C 'C'
#define Key_D 'D'
#define Key_E 'E'
#define Key_F 'F'
#define Key_G 'G'
#define Key_H 'H'
#define Key_I 'I'
#define Key_J 'J'
#define Key_K 'K'
#define Key_L 'L'
#define Key_M 'M'
#define Key_N 'N'
#define Key_O 'O'
#define Key_P 'P'
#define Key_Q 'Q'
#define Key_R 'R'
#define Key_S 'S'
#define Key_T 'T'
#define Key_U 'U'
#define Key_V 'V'
#define Key_W 'W'
#define Key_X 'X'
#define Key_Y 'Y'
#define Key_Z 'Z'
#define Key_1 '1'
#define Key_2 '2'
#define Key_3 '3'
#define Key_4 '4'
#define Key_5 '5'
#define Key_6 '6'
#define Key_7 '7'
#define Key_8 '8'
#define Key_9 '9'
#define Key_0 '0'
#define Key_Space ' '
#define Key_Control VK_CONTROL
#define Key_Alt VK_MENU
#define Key_Shift VK_SHIFT
#define Key_Tab VK_TAB
#define Key_Escape VK_ESCAPE
#define Key_Delete VK_DELETE
#define MouseButton_Right VK_RBUTTON
#define MouseButton_Left VK_LBUTTON
#define MouseButton_Middle VK_MBUTTON

//[KR][NOTE]Custom Key_s
#define MyKey Key_Space

#pragma endregion

//[KR][NOTE] Deadzones for the GamePad's joysticks
#define deadzoneX 0.1f
#define deadzoneY 0.1f

namespace Sapphire 
{
	class InputManager 
	{
	public:
		InputManager();
		~InputManager();

		//[KR][NOTE] Checks the connection of connected or disconnected GamePads and gets the input state
		void CheckConnectionGamePads();

		//[KR][NOTE] Converts the button data of the input state to Key_States
		void UpdateInputGamePads();

		void UpdateInputKeyboardMouse();	//[KR][NOTE] Updates the status of all the Key_s
		void PrintActiveKeys();				//[KR][NOTE] Prints the status of active Key_s
	
		private:
			/*enum Key_ {
				Key_A = 'A',
				Key_B = 'B',
				Key_C = 'C',
				Key_D = 'D',
				Key_E = 'E',
				Key_F = 'F',
				Key_G = 'G',
				Key_H = 'H',
				Key_I = 'I',
				Key_J = 'J',
				Key_K = 'K',
				Key_L = 'L',
				Key_M = 'M',
				Key_N = 'N',
				Key_O = 'O',
				Key_P = 'P',
				Key_Q = 'Q',
				Key_R = 'R',
				Key_S = 'S',
				Key_T = 'T',
				Key_U = 'U',
				Key_V = 'V',
				Key_W = 'W',
				Key_X = 'X',
				Key_Y = 'Y',
				Key_Z = 'Z',
				Key_1 = '1',
				Key_2 = '2',
				Key_3 = '3',
				Key_4 = '4',
				Key_5 = '5',
				Key_6 = '6',
				Key_7 = '7',
				Key_8 = '8',
				Key_9 = '9',
				Key_0 = '0',
				Key_Space = ' ',
				Key_Control = VK_CONTROL,
				Key_Alt = VK_MENU,
				Key_Shift = VK_SHIFT,
				Key_Tab = VK_TAB,
				Key_Escape = VK_ESCAPE

			};*/
			//[KR][NOTE] storing all the Key_values in an array for iteration in the update
		std::array<int, 46> allKeyValues = {
			Key_A,
			Key_B,
			Key_C,
			Key_D,
			Key_E,
			Key_F,
			Key_G,
			Key_H,
			Key_I,
			Key_J,
			Key_K,
			Key_L,
			Key_M,
			Key_N,
			Key_O,
			Key_P,
			Key_Q,
			Key_R,
			Key_S,
			Key_T,
			Key_U,
			Key_V,
			Key_W,
			Key_X,
			Key_Y,
			Key_Z,
			Key_1,
			Key_2,
			Key_3,
			Key_4,
			Key_5,
			Key_6,
			Key_7,
			Key_8,
			Key_9,
			Key_0,
			Key_Space,
			Key_Control,
			Key_Alt,
			Key_Shift,
			Key_Tab,
			Key_Escape,
			Key_Delete,
			MouseButton_Right,
			MouseButton_Left,
			MouseButton_Middle
		};

		//[KR][NOTE] stores the state of the Key_, only one of them can be true at the same time
		struct KeyState {
			bool isPressed = false;		//on Enter
			bool isHeld = false;		//on Stay
			bool isReleased = false;	//on Exit
		};

	public:

		struct CursorPos
		{
			double x;
			double y;
			CursorPos()
			{
				x = 0.0;
				y = 0.0;
			};
			CursorPos(const double newX, const double newY)
			{
				x = newX;
				y = newY;
			}
			CursorPos operator-(CursorPos cp);
		} cursorPosScreen, cursorPosWorld, cursorScreenLastPos, cursorScreenDelta;

		//[KR][NOTE] Stores everything related to a GamePad Controller
		struct GamePad {
			XINPUT_STATE XinputState;
			KeyState A;
			KeyState B;
			KeyState X;
			KeyState Y;
			KeyState DPADUP;
			KeyState DPADDOWN;
			KeyState DPADRIGHT;
			KeyState DPADLEFT;
			KeyState START;
			KeyState BACK;
			KeyState LB;
			KeyState RB;
			KeyState L3;
			KeyState R3;

			DirectX::XMFLOAT2 LEFTSTICK = { 0, 0 };
			DirectX::XMFLOAT2 RIGHTSTICK = { 0, 0 };
			float LEFTTRIGGER = 0;
			float RIGHTTRIGGER = 0;

			bool isConnected = false;
		}GamePads_[4];
		//GamePad GamePads_[4];


		HWND hwnd;

		//[KR][NOTE] Updates the state of the button Key_States according to the last frame
		void UpdateGamePadButtonKeyState(KeyState &Button, bool ButtonIsActive);

		//[KR][NOTE] Vibrates the selected GamePad, a null vibration is called in the InputManager's destructor
		void VibrateGamePad(int port, float leftMotor, float rightMotor);

		//[KR][NOTE] Prints some button data of the GamePad to the console when active
		void PrintActiveGamePads();
		
		//[KR][NOTE] map that stores all the states of the Key_s
		std::map <int, KeyState> AllKeyStates_;

	};
	extern InputManager inputManager_;
}
