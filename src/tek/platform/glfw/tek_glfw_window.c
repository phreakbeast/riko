#include "../tek_window.hpp"

#include "GLFW/glfw3.h"

namespace platform
{
    bool window_open(u32 width, u32 height, const char* title, bool fullscreen)
    {
        return true;
    }

    void window_destroy()
    {

    }
    
    void window_toggle_fullscreen()
    {
    
    }
    
    void window_swap_buffers()
    {
    
    }
    
    bool window_update()
    {
        return true;
    }
    
    bool window_should_close()
    {
        return false;
    }
    
    bool window_is_key_down(Key key)
    {
        return false;
    }
    
    bool window_is_mouse_down(MouseButton button)
    {
        return false;
    }
    
    int window_get_mouse_x()
    {
        return 0;
    }
    
    int window_get_mouse_y()
    {
        return 0;
    }
    
    int window_get_mouse_wheel()
    {
        return 0;
    }
    
    void window_set_cursor_pos(int x, int y)
    {
    
    }
    
    void window_show_cursor(int val)
    {
    
    }
    
    void window_resize_callback(void(*f)(u32, u32))
    {
    
    }
}