#ifndef _TEK_PICKING_H
#define _TEK_PICKING_H

#include "../utils/tek_core.h"
#include "tek_opengl.h"
#include "tek_mesh.h"

typedef struct pick_vertex_data_s
{
    Vec3 position;
    Vec3 color;        
}PickVertexData;

PickVertexData pick_vertex_data_create(Vec3 position, Vec3 color);

typedef struct pick_mesh_s
{
    u32 vao;
    u32 vbo[5];

    u32 num_vertices;
    u32 num_indices;        
}PickMesh;

void pick_mesh_destroy(PickMesh* mesh);

void pick_mesh_render_instanced(PickMesh* mesh, Mat4* mvp_mats, Mat4* world_mats, Vec3* colors, u32 num_instances);
void pick_mesh_render(PickMesh* mesh);

PickMesh* pick_mesh_create(PickVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices);

typedef struct pixel_info_S
{
    float object_id;
    float draw_id;
    float prim_id;        
}PixelInfo;

typedef struct picking_texture_s
{
    u32 fbo;
    u32 picking_texture;
    u32 depth_texture;

    int width;
    int height;        
}PickingTexture;

void pick_texture_destroy(PickingTexture* texture);

void pick_texture_enable_drawing(PickingTexture* texture);
void pick_texture_disable_drawing(PickingTexture* texture);
void pick_texture_bind_for_reading(PickingTexture* texture, GLenum texture_unit);

PixelInfo pick_texture_get_pixel(PickingTexture* texture, u32 x, u32 y);

PickingTexture* pick_texture_create(int width, int height);

#endif