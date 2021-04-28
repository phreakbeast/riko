#ifndef _TEK_FOLIAGE_HPP
#define _TEK_FOLIAGE_HPP

#include "tek_texture.hpp"
#include "tek_rect.hpp"
#include "../utils/tek_core.hpp"

typedef struct foliage_S
{
    u32 vao;
    u32 vbo;
    u32 num_positions;        
}Foliage;

Foliage* foliage_create(Vec3* positions, u32 num_positions, Rect rect, Rect uv);
void foliage_destroy(Foliage* foliage);
void foliage_render(Foliage* foliage);

#endif
