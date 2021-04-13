#ifndef _TEK_WATER_H
#define _TEK_WATER_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"

typedef struct water_vertex_data_s
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;        
}WaterVertexData;

WaterVertexData water_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal);

typedef struct water_s
{    
    u32 vao;
    u32 vbo;
    u32 ibo;
    u32 width;
    u32 height;
    u32 num_vertices;
    u32 num_indices;        
}Water;

Water* water_create(u32 width, u32 height, float y);
void water_destroy(Water* water);
void water_render(Water* water);

#endif