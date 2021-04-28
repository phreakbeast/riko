#include "tek_win32_gamepad.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include <climits>

#pragma comment(lib, "XInput.lib")

//XINPUT_GAMEPAD_DPAD_UP          0x00000001
//XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
//XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
//XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
//XINPUT_GAMEPAD_START            0x00000010
//XINPUT_GAMEPAD_BACK             0x00000020
//XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
//XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
//XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
//XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
//XINPUT_GAMEPAD_A                0x1000
//XINPUT_GAMEPAD_B                0x2000
//XINPUT_GAMEPAD_X                0x4000
//XINPUT_GAMEPAD_Y                0x8000

/*
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);
};

CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}
*/

XGamepadState get_xgamepad_state()
{
	XGamepadState state;

	XINPUT_STATE controller_state;
	int controller_id = 0;

	//Check if connected
	// Zeroise the state
	ZeroMemory(&controller_state, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(controller_id, &controller_state);

	if (Result == ERROR_SUCCESS)
	{
		//connected
		state.connected = true;
	}
	else
	{
		//not connected
		state.connected = false;
		return state;
	}

	state.buttons[GAMEPAD_BUTTON_A] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	state.buttons[GAMEPAD_BUTTON_B] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	state.buttons[GAMEPAD_BUTTON_X] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	state.buttons[GAMEPAD_BUTTON_Y] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	state.buttons[GAMEPAD_BUTTON_LSHOULDER] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	state.buttons[GAMEPAD_BUTTON_RSHOULDER] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	state.buttons[GAMEPAD_BUTTON_LTHUMB] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
	state.buttons[GAMEPAD_BUTTON_RTHUMB] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
	state.buttons[GAMEPAD_BUTTON_START] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
	state.buttons[GAMEPAD_BUTTON_SELECT] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
	state.buttons[GAMEPAD_BUTTON_DPAD_LEFT] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	state.buttons[GAMEPAD_BUTTON_DPAD_RIGHT] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
	state.buttons[GAMEPAD_BUTTON_DPAD_UP] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	state.buttons[GAMEPAD_BUTTON_DPAD_DOWN] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
	state.axis[GAMEPAD_AXIS_LEFTX] = controller_state.Gamepad.sThumbLX / (float)SHRT_MAX;
	state.axis[GAMEPAD_AXIS_LEFTY] = controller_state.Gamepad.sThumbLY / (float)SHRT_MAX;
	state.axis[GAMEPAD_AXIS_RIGHTX] = controller_state.Gamepad.sThumbRX / (float)SHRT_MAX;
	state.axis[GAMEPAD_AXIS_RIGHTY] = controller_state.Gamepad.sThumbRY / (float)SHRT_MAX;
	state.axis[GAMEPAD_AXIS_LTRIGGER] = controller_state.Gamepad.bLeftTrigger / (float)SHRT_MAX;
	state.axis[GAMEPAD_AXIS_RTRIGGER] = controller_state.Gamepad.bRightTrigger / (float)SHRT_MAX;

	return state;
}
