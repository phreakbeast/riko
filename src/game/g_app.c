
#include "../tek/tek_application.h"
#include "g_game.h"
#include "gui/g_gui.h"

void app_destroy()
{
    game_destroy();
    gui_destroy();
}

bool app_init(u32 width, u32 height)
{
    if(!gui_init())
        return false;
    
    if (!game_init(width, height))
        return false;

    return true;
}

void app_update(float delta)
{
    game_update(delta);
}

void app_resize(u32 width, u32 height)
{
    game_resize(width, height);
}


int main(int argc, char *args[])
{
    application_set_init_callback(app_init);
    application_set_update_callback(app_update);
    application_set_resize_callback(app_resize);

    if(!application_init(1280, 720, "Argos", false))
        return 1;

    application_start_mainloop();

    app_destroy();
    application_destroy();
    return 0;
}