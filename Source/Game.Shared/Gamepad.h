#pragma once

#include <Xinput.h>

/**
	Gamepad class for XBox controller input. Supports up to 4 controllers. Input id (0-3) when creating gamepad object.
	Analogue Sticks go from -1 to 1 in the X and Y direction.
	Triggers have a value from 0 to 1

	Functionality allows for:
	Creating a gamepad at a given port number,
	Getting analogue/trigger values,
	Checking if button is pressed

	Face buttons:
	XINPUT_GAMEPAD_A
	XINPUT_GAMEPAD_B
	XINPUT_GAMEPAD_X
	XINPUT_GAMEPAD_Y

	D-pad buttons:
	XINPUT_GAMEPAD_DPAD_LEFT
	XINPUT_GAMEPAD_DPAD_RIGHT
	XINPUT_GAMEPAD_DPAD_UP
	XINPUT_GAMEPAD_DPAD_DOWN

	Shoulder buttons:
	XINPUT_GAMEPAD_LEFT_SHOULDER
	XINPUT_GAMEPAD_RIGHT_SHOULDER

	Analogue buttons:
	XINPUT_GAMEPAD_LEFT_THUMB
	XINPUT_GAMEPAD_RIGHT_THUMB

	Center buttons:
	XINPUT_GAMEPAD_BACK
	XINPUT_GAMEPAD_START
*/

namespace KatBall
{
	class Gamepad
	{
	public:
		Gamepad(int controllerId) : mControllerId(controllerId), mDeadzoneX(0.2f), mDeadzoneY(0.2f) {}
		Gamepad(int controllerId, float dzX, float dzY) : mControllerId(controllerId), mDeadzoneX(dzX), mDeadzoneY(dzY) {}

		float leftStickX;
		float leftStickY;
		float rightStickX;
		float rightStickY;
		float leftTrigger;
		float rightTrigger;

		XINPUT_GAMEPAD *GetState();
		bool CheckConnection();
		bool Refresh();
		bool IsPressed(WORD button);
		int GetPort();

	private:
		int mControllerId;
		XINPUT_STATE mState;
		WORD mCurrentButton;

		float mDeadzoneX;
		float mDeadzoneY;
	};
}