#ifndef _G_GAME_HPP
#define _G_GAME_HPP

#include "../tek/utils/tek_core.hpp"
#include "g_map.hpp"


bool game_init(u32 width, u32 height);

void game_destroy();

void game_update(float delta);

void game_resize(u32 width, u32 height);

#endif
