#include "tek_application.h"

#include "platform/tek_window.h"
#include "platform/tek_input.h"
#include "platform/tek_clock.h"
#include "graphics/tek_drawing.h"

static bool g_is_running = true;

static float g_update_timer = 0;
static float g_update_ticks = 1.0f / 60.0f;
static float g_timer = 0;

static u32 g_frames = 0;
static u32 g_updates = 0;
static u32 g_fps = 0;
static u32 g_ups = 0;

static float g_delta = 0;

static Stopwatch g_ups_timer;
static Stopwatch g_delta_timer;
static Stopwatch g_frame_timer;

static u32 g_width = 0;
static u32 g_height = 0;

static void on_resize(u32 width, u32 height);

static bool (*g_on_init_callback)(u32, u32) = NULL;

static void (*g_on_update_callback)(float) = NULL;

static void (*g_on_resize_callback)(u32, u32) = NULL;

void application_destroy()
{    
    renderer_destroy();
    input_destroy();
    window_destroy();
}

bool application_init(u32 width, u32 height, const char *title, bool fullscreen)
{
    g_width = width;
    g_height = height;

    window_resize_callback(on_resize);

    if (!window_open(width, height, title, 0))
        return false;

    if (!input_init())
        return false;

    load_gl_extensions();

    if (!renderer_init(width, height, width, height))
        return false;

    time_init();    

    bool result = true;
    if (g_on_init_callback)
    {
        result = g_on_init_callback(width, height);
    }
    return result;
}

void application_start_mainloop()
{    
    stopwatch_start(&g_ups_timer);
    stopwatch_start(&g_delta_timer);

    while (g_is_running)
    {
        RenderStats *stats = renderer_get_stats();
        double seconds = stopwatch_get_seconds(&g_ups_timer);
        if (seconds - g_update_timer > g_update_ticks)
        {
            stopwatch_start(&g_frame_timer);
            g_delta = (float) stopwatch_get_seconds(&g_delta_timer);
            stopwatch_start(&g_delta_timer);
            if (!window_update())
            {
                break;
            }
            input_update();

            KeyboardState *kstate = input_get_key_state();
            if (kstate->keys_pressed[KEY_ESCAPE])
            {
                break;
            }

            g_is_running = !window_should_close();

            renderer_start_frame();
            
            if (g_on_update_callback)
            {
                g_on_update_callback(g_delta);
            }            
    
            renderer_end_frame();
            window_swap_buffers();
            
            stats->frame_time = stopwatch_get_seconds(&g_frame_timer);
            //printf("%f\n",s);
            g_update_timer += g_update_ticks;
            g_frames++;
            g_updates++;
        }

        if (seconds - g_timer > 1.0f)
        {
            g_timer += 1.0f;
            //tick;

            g_fps = g_frames;
            g_ups = g_updates;
            g_frames = 0;
            g_updates = 0;
            stats->fps = g_fps;
            stats->delta = g_delta;
        }
    }
}

void application_set_init_callback(bool(*f)(u32, u32))
{
    g_on_init_callback = f;
}

void application_set_update_callback(void(*f)(float))
{
    g_on_update_callback = f;
}

void application_set_resize_callback(void(*f)(u32, u32))
{
    g_on_resize_callback = f;
}

static void on_resize(u32 width, u32 height)
{
    g_width = width;
    g_height = height;

    renderer_resize(width, height);

    if (g_on_resize_callback)
    {
        g_on_resize_callback(width, height);
    }
}