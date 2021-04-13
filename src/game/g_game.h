#ifndef _G_GAME_H
#define _G_GAME_H

#include "../tek/utils/tek_core.h"
#include "g_map.h"


bool game_init(u32 width, u32 height);

void game_destroy();

void game_update(float delta);

void game_resize(u32 width, u32 height);

#endif
