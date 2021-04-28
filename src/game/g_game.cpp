#include "g_game.hpp"

#include "../tek/graphics/tek_drawing.hpp"
#include "../tek/platform/tek_input.hpp"

#include "g_map.hpp"
#include "gui/g_gui.hpp"

#include <stdlib.h>

static u32 g_width;
static u32 g_height;

static BitmapFont* g_font = NULL;

static ShadowMapFBO* g_shadow_map = NULL;

static GuiFrame* g_frame;

static Camera g_cam;
static Map* g_map = NULL;
static bool g_show_shadow_map = false;

bool game_init(u32 width, u32 height)
{       
    g_width = width;
    g_height = height;        
    
    g_font = bitmapfont_load("data/fonts/test.font");
    if(!g_font)
    {
        return false;
    }                            
    
    g_shadow_map = shadow_map_create(1024, 1024);

    g_map = map_load();
    
    Mat4 projection = mat4_perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    g_cam = cam_create(vec3_create(0, 0, 0), projection);

    g_frame = gui_frame_create();
    g_frame->x = width - 200.0f;
    g_frame->y = 0;
    g_frame->width = 200;
    g_frame->height = height;

    GuiWidget* btn = gui_widget_create_button();
    btn->button->x = 5;
    btn->button->y = 5;
    btn->button->width = 50;
    btn->button->height = 50;

    GuiWidget* btn2 = gui_widget_create_button();
    btn2->button->x = 5;
    btn2->button->y = 60;
    btn2->button->width = 50;
    btn2->button->height = 50;

    gui_frame_add_widget(g_frame, btn);
    gui_frame_add_widget(g_frame, btn2);

    Texture* gui_texture = gui_get_texture();
    GuiWidget* img = gui_widget_create_image();
    img->image->texture = gui_texture;
    img->image->source = rect_uv(0,0,100,100,gui_texture->width,gui_texture->height);
    img->image->x = 5;
    img->image->y = 115;
    img->image->width = 50;
    img->image->height = 50;
    gui_frame_add_widget(g_frame, img);

    GuiWidget* text = gui_widget_create_text();
    text->text->x = 5;
    text->text->y = 170;
    gui_text_set_text(text->text, "Test text");
    gui_frame_add_widget(g_frame, text);
    
    GuiWidget* checkbox = gui_widget_create_checkbox();
    checkbox->checkbox->x = 5;
    checkbox->checkbox->y = 220;
    checkbox->checkbox->width = 15;
    checkbox->checkbox->height = 15;
    gui_frame_add_widget(g_frame, checkbox);

    GuiWidget* slider = gui_widget_create_slider();
    slider->slider->x = 5;
    slider->slider->y = 240;
    gui_frame_add_widget(g_frame, slider);

    gui_add_frame(g_frame);

    return true;
}

void game_destroy()
{       
    shadow_map_destroy(g_shadow_map);
    free(g_shadow_map);

    map_destroy(g_map);
    free(g_map);

    bitmapfont_destroy(g_font);
    free(g_font);    
}

void game_update(float delta)
{    
    KeyboardState* kstate = input_get_key_state();
    MouseState* mstate = input_get_mouse_state();
   
    if(kstate->keys_pressed[KEY_G])
    {
        renderer_toggle_wireframe();
    }

    if (kstate->keys_down[KEY_A])
    {
        cam_rotate_y(&g_cam, delta * 50);
    }
    if (kstate->keys_down[KEY_D])
    {
        cam_rotate_y(&g_cam, -delta * 50);
    }
    if (kstate->keys_down[KEY_W])
    {
        cam_rotate_x(&g_cam, delta * 50);
    }
    if (kstate->keys_down[KEY_S])
    {
        cam_rotate_x(&g_cam, -delta * 50);
    }
    if (kstate->keys_down[KEY_Q])
    {
        cam_zoom(&g_cam,-delta * 10);
    }
    if (kstate->keys_down[KEY_E])
    {
        cam_zoom(&g_cam,delta * 10);
    }
    if (kstate->keys_pressed[KEY_L])
    {
        g_show_shadow_map = !g_show_shadow_map;
    }
    cam_calc(&g_cam);

    renderer_enable_depth_test();
    
    //render shadows
    shadow_map_bind_for_writing(g_shadow_map);
    renderer_viewport(g_shadow_map->width,g_shadow_map->height);
    renderer_clear(color_black());
    map_render_shadows(g_map, &g_cam);
    shadow_map_unbind();

    renderer_viewport(g_width,g_height);
    renderer_clear(color_white());
    shadow_map_bind_for_reading(g_shadow_map, 3);
    map_update(g_map, delta, &g_cam);
    
    renderer_disable_depth_test();

    if(g_show_shadow_map) {
        shadow_map_bind_for_reading(g_shadow_map, 0);
        Mat4 ortho = mat4_ortho(0, (float) g_width, 0, (float) g_height, -1, 1);
        ortho = mat4_transposed(&ortho);
        renderer_render_sprite(rect_create(0, 50, 256, 256), rect_create(0, 0, 1, 1), g_shadow_map->shadow_map, &ortho);
    }

    sb_begin();
    gui_update(delta);
    
    char fps_str[256];
    char sprites_str[256];
    char drawcalls_str[256];
    char delta_str[256];
    
    RenderStats* stats = renderer_get_stats();
    sprintf(fps_str, "fps: %u frame: %f", stats->fps, stats->frame_time);
    sprintf(sprites_str, "num_sprites: %u", stats->num_sprites);
    sprintf(drawcalls_str, "drawcalls: %u", stats->num_drawcalls);
    sprintf(delta_str, "delta: %f", stats->delta);    
    
    sb_render_text(fps_str, g_font, 5, 5, color_black(), 0);
    sb_render_text(sprites_str, g_font, 5, 15, color_black(), 0);
    sb_render_text(drawcalls_str, g_font, 5, 25, color_black(), 0);
    sb_render_text(delta_str, g_font, 5, 35, color_black(), 0);
        
    sb_end();
    sb_flush();
}

void game_resize(u32 width, u32 height)
{
    g_width = width;
    g_height = height;        

    Mat4 projection = mat4_perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    g_cam.projection = projection;

    g_frame->x = (float)width - g_frame->width;
    g_frame->height = (float)height;
}