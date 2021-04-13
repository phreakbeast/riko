#ifndef _TEK_GEOMETRY_H
#define _TEK_GEOMETRY_H

#include "tek_color.h"
#include "tek_mesh.h"
#include "tek_picking.h"

typedef struct mesh_buffer_s
{
    MeshVertexData* res_vertices;
    u32* res_indices;
    u32 num_vertices;
    u32 num_indices;
}MeshBuffer;

void mesh_buffer_init(MeshBuffer* buffer);
void mesh_buffer_destroy(MeshBuffer* buffer);

void mesh_buffer_add_plane(MeshBuffer* buffer, float width, float depth, Color color);
void mesh_buffer_add_box(MeshBuffer* buffer, float width, float height, float depth, Color color, Vec3 offset);
void mesh_buffer_add_cylinder(MeshBuffer* buffer, float radius, int num_slices, float height, bool with_positions, bool with_uv, bool with_normals);
void mesh_buffer_add_sphere(MeshBuffer* buffer, float rad, int rings, int sectors, int inverted, Color color);
void mesh_buffer_add_pyramid(MeshBuffer* buffer, Vec3 position, float width, float height, Color color);
bool mesh_buffer_load_mesh(MeshBuffer* buffer, const char* filename);

void mesh_buffer_add_vertices(MeshBuffer* buffer, MeshVertexData* vertices,u32 num_vertices, u32* indices, u32 num_indices);

Mesh* mesh_buffer_to_mesh(MeshBuffer* buffer);
PickMesh* mesh_buffer_to_pickmesh(MeshBuffer* buffer);

#endif