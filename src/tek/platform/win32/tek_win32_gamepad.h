#ifndef _TEK_GAMEPAD_H
#define _TEK_GAMEPAD_H

#include "../tek_keys.h"

typedef struct xgamepad_state_t
{
	bool connected;
	bool buttons[NUM_GAMEPAD_BUTTONS];
	float axis[NUM_GAMEPAD_AXIS];
}XGamepadState;

XGamepadState get_xgamepad_state();

#endif
