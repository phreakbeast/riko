#ifndef _TEK_DRAWING_H
#define _TEK_DRAWING_H

#include "../utils/tek_core.h"
#include "tek_font.h"
#include "tek_transform.h"
#include "tek_framebuffer.h"
#include "tek_rect.h"
#include "tek_color.h"
#include "tek_shader.h"
#include "tek_spritebatch.h"
#include "tek_mesh.h"
#include "tek_picking.h"
#include "tek_camera.h"
#include "tek_terrain.h"
#include "tek_shape.h"
#include "tek_light.h"
#include "tek_water.h"
#include "tek_material.h"

typedef struct
{
    float delta;
    u32 fps;
    float frame_time;
    u32 num_sprites;
    u32 num_drawcalls;
}RenderStats;


bool renderer_init(u32 win_width, u32 win_height, u32 target_width, u32 target_height);

void renderer_destroy();

void renderer_start_frame();

void renderer_end_frame();

void renderer_bind_framebuffer(Framebuffer *buffer, Color color);

void renderer_unbind_framebuffer();

void renderer_viewport(u32 width, u32 height);

void renderer_disable_depth_test();
void renderer_enable_depth_test();
void renderer_enable_wireframe();
void renderer_disable_wireframe();
void renderer_toggle_wireframe();

void renderer_clear(Color color);

RenderStats *renderer_get_stats();

void renderer_resize(u32 width, u32 height);

void renderer_render_sprite(Rect dest, Rect src, u32 texture_id, Mat4 *ortho);

void renderer_draw_terrain(Terrain *terrain, Camera *cam, SunLight* light);

void renderer_draw_mesh(Mesh *mesh, Material* material, Transform* transform, Camera *cam, SunLight* light, PointLight* point_lights, u32 num_point_lights);
void renderer_draw_mesh_shadow(Mesh *mesh, Transform* transform, Mat4* mvp);
void renderer_draw_mesh_instanced(Mesh *mesh, Texture *texture, Transform* transforms, u32 num_transforms, Camera *cam, SunLight* light);

void renderer_draw_pick_mesh(PickMesh *mesh, Transform* transform, Camera *cam);
void renderer_draw_pick_mesh_instanced(PickMesh *mesh, Transform* transforms, u32 num_transforms, Vec3* colors, Camera *cam);

void renderer_draw_shape(Shape* shape, Mat4* transform, Camera* cam);

void renderer_draw_water(Water *water, Camera *cam);

#endif
