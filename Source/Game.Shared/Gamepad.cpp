#include "pch.h"

namespace KatBall
{
	XINPUT_GAMEPAD* Gamepad::GetState()
	{
		return &mState.Gamepad;
	}

	bool Gamepad::CheckConnection()
	{
		if (mControllerId < XUSER_MAX_COUNT)
		{
			ZeroMemory(&mState, sizeof(XINPUT_STATE));

			return (XInputGetState(mControllerId, &mState) == ERROR_SUCCESS);
		}

		return false;
	}

	// Returns false if the controller has been disconnected
	bool Gamepad::Refresh()
	{
		if (CheckConnection())
		{
			float normLX = fmaxf(-1, (float)mState.Gamepad.sThumbLX / 32767);
			float normLY = fmaxf(-1, (float)mState.Gamepad.sThumbLY / 32767);

			leftStickX = (abs(normLX) < mDeadzoneX ? 0 : (abs(normLX) - mDeadzoneX) * (normLX / abs(normLX)));
			leftStickY = (abs(normLY) < mDeadzoneY ? 0 : (abs(normLY) - mDeadzoneY) * (normLY / abs(normLY)));

			if (mDeadzoneX > 0) leftStickX *= 1 / (1 - mDeadzoneX);
			if (mDeadzoneY > 0) leftStickY *= 1 / (1 - mDeadzoneY);

			float normRX = fmaxf(-1, (float)mState.Gamepad.sThumbRX / 32767);
			float normRY = fmaxf(-1, (float)mState.Gamepad.sThumbRY / 32767);

			rightStickX = (abs(normRX) < mDeadzoneX ? 0 : (abs(normRX) - mDeadzoneX) * (normRX / abs(normRX)));
			rightStickY = (abs(normRY) < mDeadzoneY ? 0 : (abs(normRY) - mDeadzoneY) * (normRY / abs(normRY)));

			if (mDeadzoneX > 0) rightStickX *= 1 / (1 - mDeadzoneX);
			if (mDeadzoneY > 0) rightStickY *= 1 / (1 - mDeadzoneY);

			leftTrigger = (float)mState.Gamepad.bLeftTrigger / 255;
			rightTrigger = (float)mState.Gamepad.bRightTrigger / 255;

			return true;
		}
		return false;
	}

	bool Gamepad::IsPressed(WORD button)
	{
		return (mState.Gamepad.wButtons & button) != 0;
	}
}