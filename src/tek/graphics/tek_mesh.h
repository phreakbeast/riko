#ifndef _TEK_MESH_H
#define _TEK_MESH_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"

typedef struct mesh_vertex_data_S
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
    Vec4 color; 
}MeshVertexData;

MeshVertexData mesh_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color);

typedef struct mesh_s
{    
    Vec3 min;
    Vec3 max;
    Vec3 center;

    u32 vao;
    u32 vbo[4];

    u32 num_vertices;
    u32 num_indices;        
}Mesh;

void mesh_destroy(Mesh* mesh);

void mesh_create(Mesh* mesh, MeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices, bool calc_normals);

void mesh_render(Mesh* mesh);
void mesh_render_instanced(Mesh* mesh, Mat4* mvp_mats, Mat4* world_mats, u32 num_instances);

Mesh* mesh_load(const char* filename);


#endif