#include "tek_input.h"

#include "tek_window.h"

static KeyboardState g_key_state;
static MouseState g_mouse_state;
//static GamepadState g_gamepad_state;

bool input_init()
{
    for(u32 i = 0; i < NUM_KEYS; ++i)
    {
        g_key_state.keys_down[i] = 0;
        g_key_state.keys_pressed[i] = 0;
        g_key_state.keys_released[i] = 0;
    }
    
    g_mouse_state.mx = 0;
    g_mouse_state.my = 0;
    g_mouse_state.old_mx = 0;
    g_mouse_state.old_my = 0;
    g_mouse_state.wheel = 0;
    for(u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
        g_mouse_state.buttons_down[i] = 0;
        g_mouse_state.buttons_pressed[i] = 0;
        g_mouse_state.buttons_released[i] = 0;
    }
    
    return true;
}

void input_destroy()
{

}

void input_update()
{    
    for(u32 i = 0; i < NUM_KEYS; ++i)
    {
        int down = window_is_key_down((Key)i);
        
        if(down)
        {
            
            if(g_key_state.keys_down[i])
            {
                g_key_state.keys_pressed[i] = false;
            }
            else
            {
                g_key_state.keys_pressed[i] = true;
            }
            g_key_state.keys_down[i] = true;
            g_key_state.keys_released[i] = false;
        }
        else
        {
            if(g_key_state.keys_down[i])
            {
                g_key_state.keys_released[i] = true;
            }
            else
            {
                g_key_state.keys_released[i] = false;
            }
            g_key_state.keys_down[i] = false;
            g_key_state.keys_pressed[i] = false;
        }
        
    }
    
    g_mouse_state.old_mx = g_mouse_state.mx;
    g_mouse_state.old_my = g_mouse_state.my;
    
    g_mouse_state.mx = window_get_mouse_x();
    g_mouse_state.my = window_get_mouse_y();
    g_mouse_state.wheel = window_get_mouse_wheel();
    
    for(u32 i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
        int down = window_is_mouse_down((MouseButton)i);
        
        if(down)
        {
            if(g_mouse_state.buttons_down[i])
            {
                g_mouse_state.buttons_pressed[i] = false;
            }
            else
            {
                g_mouse_state.buttons_pressed[i] = true;
            }
            g_mouse_state.buttons_down[i] = true;
            g_mouse_state.buttons_released[i] = false;
        }
        else
        {
            if(g_mouse_state.buttons_down[i])
            {
                g_mouse_state.buttons_released[i] = true;
            }
            else
            {
                g_mouse_state.buttons_released[i] = false;
            }
            g_mouse_state.buttons_down[i] = false;
            g_mouse_state.buttons_pressed[i] = false;
        }
    }
    
    /*
    XGamepadState xstate = get_xgamepad_state();
    if (xstate.connected)
    {
        for (u32 i = 0; i < NUM_GAMEPAD_BUTTONS; ++i)
        {
            bool down = xstate.buttons[(GamepadButton)i];
            if (down)
            {
                if (g_gamepad_state.buttons_down[i])
                {
                    g_gamepad_state.buttons_pressed[i] = false;
                }
                else
                {
                    g_gamepad_state.buttons_pressed[i] = true;
                }
                g_gamepad_state.buttons_down[i] = true;
                g_gamepad_state.buttons_released[i] = false;
            }
            else
            {
                if (g_gamepad_state.buttons_down[i])
                {
                    g_gamepad_state.buttons_released[i] = true;
                }
                else
                {
                    g_gamepad_state.buttons_released[i] = false;
                }
                g_gamepad_state.buttons_down[i] = false;
                g_gamepad_state.buttons_pressed[i] = false;
            }
        }
     */
    
    //g_gamepad_state.axis[GAMEPAD_AXIS_LEFTX] = xstate.axis[GAMEPAD_AXIS_LEFTX];
    //g_gamepad_state.axis[GAMEPAD_AXIS_LEFTY] = xstate.axis[GAMEPAD_AXIS_LEFTY];
    //g_gamepad_state.axis[GAMEPAD_AXIS_RIGHTX] = xstate.axis[GAMEPAD_AXIS_RIGHTX];
    //g_gamepad_state.axis[GAMEPAD_AXIS_RIGHTY] = xstate.axis[GAMEPAD_AXIS_RIGHTY];
    //g_gamepad_state.axis[GAMEPAD_AXIS_LTRIGGER] = xstate.axis[GAMEPAD_AXIS_LTRIGGER];
    //g_gamepad_state.axis[GAMEPAD_AXIS_RTRIGGER] = xstate.axis[GAMEPAD_AXIS_RTRIGGER];
    
}

MouseState* input_get_mouse_state()
{
    return &g_mouse_state;
}

KeyboardState* input_get_key_state()
{
    return &g_key_state;
}

/*
GamepadState* input_get_gamepad_state()
{
    return &g_gamepad_state;
}
 */
