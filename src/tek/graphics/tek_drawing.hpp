#ifndef _TEK_DRAWING_HPP
#define _TEK_DRAWING_HPP

#include "../utils/tek_core.hpp"
#include "tek_font.hpp"
#include "tek_transform.hpp"
#include "tek_framebuffer.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"
#include "tek_shader.hpp"
#include "tek_spritebatch.hpp"
#include "tek_mesh.hpp"
#include "tek_picking.hpp"
#include "tek_camera.hpp"
#include "tek_terrain.hpp"
#include "tek_shape.hpp"
#include "tek_light.hpp"
#include "tek_water.hpp"
#include "tek_material.hpp"
#include "tek_animation.hpp"

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

void renderer_draw_mesh(Mesh *mesh, Material* material, Transform* transform, Camera *cam, SunLight* light, PointLight* point_lights, u32 num_point_lights, Mat4* light_matrix);
void renderer_draw_mesh_shadow(Mesh *mesh, Transform* transform, Mat4* mvp);
void renderer_draw_mesh_instanced(Mesh *mesh, Texture *texture, Transform* transforms, u32 num_transforms, Camera *cam, SunLight* light);

void renderer_draw_pick_mesh(PickMesh *mesh, Transform* transform, Camera *cam);
void renderer_draw_pick_mesh_instanced(PickMesh *mesh, Transform* transforms, u32 num_transforms, Vec3* colors, Camera *cam);

void renderer_draw_shape(Shape* shape, Mat4* transform, Camera* cam);

void renderer_draw_water(Water *water, Camera *cam);

void renderer_draw_animation(animation_t* anim, Material* material, Transform* transform, Camera *cam, float frame, SunLight* light, PointLight* point_lights, u32 num_point_lights);
void renderer_draw_animation_shadow(animation_t* anim, Transform* transform, float frame, Mat4* mvp);

#endif
