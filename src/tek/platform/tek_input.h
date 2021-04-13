#ifndef _TEK_INPUT_H
#define _TEK_INPUT_H

#include "../utils/tek_core.h"
#include "tek_keys.h"
//#include "win32/tek_win32_gamepad.hpp"


typedef struct  {
    bool keys_down[NUM_KEYS];
    bool keys_pressed[NUM_KEYS];
    bool keys_released[NUM_KEYS];
}KeyboardState;

typedef struct {
    int mx;
    int my;
    int old_mx;
    int old_my;
    int wheel;

    bool buttons_down[NUM_MOUSE_BUTTONS];
    bool buttons_pressed[NUM_MOUSE_BUTTONS];
    bool buttons_released[NUM_MOUSE_BUTTONS];
} MouseState;

/*
struct GamepadState
{
    bool buttons_down[NUM_GAMEPAD_BUTTONS];
    bool buttons_pressed[NUM_GAMEPAD_BUTTONS];
    bool buttons_released[NUM_GAMEPAD_BUTTONS];

    float axis[NUM_GAMEPAD_AXIS];
};
 */

bool input_init();

void input_destroy();

void input_update();

MouseState *input_get_mouse_state();

KeyboardState *input_get_key_state();
//GamepadState* input_get_gamepad_state();


#endif
