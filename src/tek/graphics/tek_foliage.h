#ifndef _TEK_FOLIAGE_H
#define _TEK_FOLIAGE_H

#include "tek_texture.h"
#include "tek_rect.h"
#include "../utils/tek_core.h"

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
