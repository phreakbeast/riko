#ifndef _TEK_GAMEPAD_HPP
#define _TEK_GAMEPAD_HPP

#include "../tek_keys.hpp"

typedef struct xgamepad_state_t
{
	bool connected;
	bool buttons[NUM_GAMEPAD_BUTTONS];
	float axis[NUM_GAMEPAD_AXIS];
}XGamepadState;

XGamepadState get_xgamepad_state();

#endif
