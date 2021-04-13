#ifndef _TEK_TERRAIN_H
#define _TEK_TERRAIN_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"

typedef struct terrain_vertex_data_s
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
    Vec4 color;    
}TerrainVertexData;

TerrainVertexData terrain_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color);

typedef struct terrain_s
{
    u32 vao;
    u32 vbo;
    u32 ibo;
    
    u32 num_vertices;
    u32 num_indices;
}Terrain;

Terrain* terrain_create(TerrainVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices);
void terrain_destroy(Terrain* terrain);

void terrain_render(Terrain* terrain);

#endif