#include "tek_drawing.h"

#include "tek_opengl.h"
#include "tek_shapebuffer.h"

#include <string.h>
#include <stdio.h>

static RenderStats g_stats;
static u32 g_drawcalls = 0;

static bool g_wireframe = false;

static Shader* g_sprite_shader;
static Shader* g_geometry_shader;
static Shader* g_geometry_instanced_shader;
static Shader* g_terrain_shader;
static Shader* g_picking_shader;
static Shader* g_picking_instanced_shader;
static Shader* g_shape_shader;
static Shader* g_water_shader;
static Shader* g_shadow_shader;

static u32 g_width;
static u32 g_height;
//static u32 g_render_width;
//static u32 g_render_height;

//static TekRect g_target_rect;
static Framebuffer* g_fb;
static Mat4 g_ortho;

static Rect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height);

void renderer_destroy()
{
    shader_destroy(g_sprite_shader);
    free(g_sprite_shader);
    shader_destroy(g_geometry_shader);
    free(g_geometry_shader);
    shader_destroy(g_geometry_instanced_shader);
    free(g_geometry_instanced_shader);
    shader_destroy(g_terrain_shader);
    free(g_terrain_shader);
    shader_destroy(g_picking_shader);
    free(g_picking_shader);
    shader_destroy(g_picking_instanced_shader);
    free(g_picking_instanced_shader);
    shader_destroy(g_shape_shader);
    free(g_shape_shader);
    shader_destroy(g_water_shader);
    free(g_water_shader);
    shader_destroy(g_shadow_shader);
    free(g_shadow_shader);
    sb_destroy();
    sbuffer_destroy();
    fb_destroy(g_fb);
    free(g_fb);
}

bool renderer_init(u32 win_width, u32 win_height, u32 target_width, u32 target_height)
{   
    printf("!!!=================================================!!!\n");
    printf("!!!=== OpenGL Stats ================================!!!\n");
    printf("!!!=================================================!!!\n");
    
    GLCall(print_gl_string(GL_RENDERER, "GL_RENDERER"));
    GLCall(print_gl_string(GL_VENDOR, "GL_VENDOR"));
    
    GLCall(print_gl_string(GL_SHADING_LANGUAGE_VERSION, "GL_SHADING_LANGUAGE_VERSION"));
    GLCall(print_gl_string(GL_VERSION, "GL_VERSION"));
    
    printf("Rendering Resolution: %dx%d\n", target_width, target_height);
    
    g_width = win_width;
    g_height = win_height;
    //g_render_width = target_width;
    //g_render_height = target_height;
    
    GLCall(glViewport(0, 0, target_width, target_height));
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    
    GLCall(glEnable(GL_DEPTH_TEST));
    
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    
    GLCall(glFrontFace(GL_CCW));
    
    GLCall(glCullFace(GL_BACK));
    
    GLCall(glEnable(GL_CULL_FACE));
    
    GLCall(glLineWidth(2));
    
    if(!sb_init(target_width, target_height, 60000))
    {
        return false;
    }
    
    if(!sbuffer_init(60000))
    {
        return false;
    }
    
    g_sprite_shader = shader_load("basic.glsl");
    if(!g_sprite_shader)
    {
        return false;
    }
    
    g_geometry_shader = shader_load("geometry.glsl");
    if (!g_geometry_shader)
    {
        return false;
    }

    g_geometry_instanced_shader = shader_load("geometry_instanced.glsl");
    if(!g_geometry_instanced_shader)
    {
        return false;
    }
    
    g_terrain_shader = shader_load("terrain.glsl");
    if(!g_terrain_shader)
    {
        return false;
    }
    
    g_picking_shader = shader_load("vertex_picking.glsl");
    if(!g_picking_shader)
    {
        return false;
    }
    
    g_picking_instanced_shader = shader_load("vertex_picking_instanced.glsl");
    if(!g_picking_instanced_shader)
    {
        return false;
    }
    
    g_shape_shader = shader_load("shape.glsl");
    if(!g_shape_shader)
    {
        return false;
    }
    
    g_water_shader = shader_load("water.glsl");
    if(!g_water_shader)
    {
        return false;
    }
    
    g_shadow_shader = shader_load("shadow_map.glsl");
    if (!g_shadow_shader)
    {
        return false;
    }

    //g_target_rect = get_target_rect(win_width, win_height, target_width, target_height);
    g_ortho = mat4_ortho(0, (float)target_width, 0, (float)target_height, -1, 1);
    g_ortho = mat4_transposed(&g_ortho);
    
    g_fb = fb_create(target_width, target_height);
    
    return true;
}

void renderer_start_frame()
{
    GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    //renderer_bind_framebuffer(&g_fb, Color::black());
    //renderer_viewport(g_render_width, g_render_height);
}

void renderer_end_frame()
{
    //renderer_unbind_framebuffer();
    //tek_fb_bind_reading(&g_fb,0);
    //renderer_viewport(g_width, g_height);
    //renderer_render_sprite(g_target_rect, tek_rect_create(0, 0, 1, 1), g_fb.tex_id, &g_ortho);
    
    g_stats.num_drawcalls = g_drawcalls;// + g_sprite_batch->drawcalls;
    g_stats.num_sprites = 0;//g_sprite_batch->num_sprites;
    
    g_drawcalls = 0;
    
    sb_reset_stats();
}

void renderer_resize(u32 width, u32 height)
{
    
    g_width = width;
    g_height = height;
    GLCall(glViewport(0, 0, width, height));
    g_ortho = mat4_ortho(0, (float)width, 0, (float)height, -1, 1);
    g_ortho = mat4_transposed(&g_ortho);
    //g_target_rect = get_target_rect(width,height,g_render_width,g_render_height);
    
    sb_resize(width, height);
}

void renderer_bind_framebuffer(Framebuffer* buffer, Color color)
{
    
    fb_bind_writing(buffer);
    GLCall(glViewport(0, 0, buffer->width, buffer->height));
    Vec4 col = color_to_vec4(color);
    GLCall(glClearColor(col.x, col.y, col.z, col.w));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void renderer_unbind_framebuffer()
{
    fb_unbind();
    //GLCall(glViewport(0, 0, g_width, g_height));
}

void renderer_viewport(u32 width, u32 height)
{
    GLCall(glViewport(0, 0, width, height));
}

void renderer_disable_depth_test()
{
    GLCall(glDisable(GL_DEPTH_TEST));
}

void renderer_enable_depth_test()
{
    GLCall(glEnable(GL_DEPTH_TEST));
}

void renderer_enable_wireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    g_wireframe = true;
}

void renderer_disable_wireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    g_wireframe = false;
}

void renderer_toggle_wireframe()
{
    if(g_wireframe)
    {
        renderer_disable_wireframe();
    }
    else
    {
        renderer_enable_wireframe();
    }
}

void renderer_clear(Color color)
{
    Vec4 col = color_to_vec4(color);
    GLCall(glClearColor(col.x, col.y, col.z, col.w));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

RenderStats* renderer_get_stats()
{
    //g_stats.num_sprites = g_sprite_batch->num_sprites;
    //g_stats.num_drawcalls = g_sprite_batch->drawcalls + g_drawcalls;
    return &g_stats;
}

typedef struct
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
} TekVertexData;

TekVertexData tek_mesh_vertex_data_create(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
    TekVertexData res;
    res.position = vec3_create(x, y, z);
    res.uv = vec2_create(u, v);
    res.normal = vec3_create(nx, ny, nz);
    return res;
}

void renderer_render_sprite(Rect dest, Rect src, u32 texture_id, Mat4* ortho)
{
    shader_bind(g_sprite_shader);
    
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
    
    shader_uniform_mat4(g_sprite_shader, 0, ortho, 1, false);
    shader_uniform_int(g_sprite_shader, 2, 1);
    
    TekVertexData vertices[4];
    u32 indices[6];
    
    float x = dest.x;
    float y = dest.y;
    float w = dest.x + dest.w;
    float h = dest.y + dest.h;
    
    Vec2 uv[4];
    uv[0].x = src.x;
    uv[0].y = src.h;
    uv[1].x = src.x;
    uv[1].y = src.y;
    uv[2].x = src.w, uv[2].y = src.y;
    uv[3].x = src.w, uv[3].y = src.h;
    
    vertices[0] = tek_mesh_vertex_data_create(x, y, 0, uv[0].x, uv[0].y, 0, 0, 0);
    vertices[1] = tek_mesh_vertex_data_create(x, h, 0, uv[1].x, uv[1].y, 0, 0, 0);
    vertices[2] = tek_mesh_vertex_data_create(w, h, 0, uv[2].x, uv[2].y, 0, 0, 0);
    vertices[3] = tek_mesh_vertex_data_create(w, y, 0, uv[3].x, uv[3].y, 0, 0, 0);
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    
    int size = 4 * sizeof(TekVertexData);
    
    u32 vao;
    u32 vbo;
    u32 ibo;
    
    GLCall(glGenVertexArrays(1, &vao));
    
    GLCall(glBindVertexArray(vao));
    
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glGenBuffers(1, &ibo));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TekVertexData),
                                 (const GLvoid*)(offsetof(TekVertexData, position))));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TekVertexData),
                                 (const GLvoid*)(offsetof(TekVertexData, uv))));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), indices, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    
    GLCall(glDeleteBuffers(1, &vbo));
    GLCall(glDeleteBuffers(1, &ibo));
    GLCall(glDeleteVertexArrays(1, &vao));
    
    g_drawcalls++;
}

void renderer_draw_mesh(Mesh *mesh, Material* material, Transform* transform, Camera *cam, SunLight* light, PointLight* point_lights, u32 num_point_lights)
{
    shader_bind(g_geometry_shader);

    if (material)
    {        
        shader_uniform_int(g_geometry_shader, 9, 1);//use texture
        shader_uniform_vec4(g_geometry_shader, 10, color_to_vec4(material->ambient_color));
        shader_uniform_vec4(g_geometry_shader, 11, color_to_vec4(material->diffuse_color));
        shader_uniform_vec4(g_geometry_shader, 12, color_to_vec4(material->specular_color));
        if (material->diffuse_map)
        {
            shader_uniform_int(g_geometry_shader, 13, 1);
            texture_bind(material->diffuse_map, 0);
        }
        else
        {
            shader_uniform_int(g_geometry_shader, 13, 0);
        }
        if (material->specular_map)
        {
            shader_uniform_int(g_geometry_shader, 14, 1);
            texture_bind(material->specular_map, 1);
        }
        else
        {
            shader_uniform_int(g_geometry_shader, 14, 0);
        }
        if (material->normal_map)
        {
            shader_uniform_int(g_geometry_shader, 15, 1);
            texture_bind(material->normal_map, 2);
        }
        else
        {
            shader_uniform_int(g_geometry_shader, 15, 0);
        }
        shader_uniform_float(g_geometry_shader, 19, material->specular_level);
        shader_uniform_float(g_geometry_shader, 20, material->specular_power);
    }
    else
    {
        shader_uniform_int(g_geometry_shader, 9, 0);//use texture
    }
    
    Mat4 mvp = mat4_mul3(&cam->projection, &cam->view, &transform->matrix);
    mvp = mat4_transposed(&mvp);
    Mat4 world = mat4_transposed(&transform->matrix);

    shader_uniform_mat4(g_geometry_shader, 0, &mvp, 1, false);
    shader_uniform_mat4(g_geometry_shader, 1, &world, 1, false);

    if (light)
    {
        shader_uniform_int(g_geometry_shader, 7, 1); //use light
        shader_uniform_vec3(g_geometry_shader, 2, light->direction);
        shader_uniform_vec3(g_geometry_shader, 3, light->color);
        shader_uniform_float(g_geometry_shader, 4, light->intensity);
        shader_uniform_vec3(g_geometry_shader, 6, cam->position);
    }
    else
    {
        shader_uniform_int(g_geometry_shader, 7, 0); //use light
    }

    shader_uniform_int(g_geometry_shader, 21, num_point_lights); //use light
    u32 light_offset = 22;
    for (u32 i = 0; i < num_point_lights; ++i)
    {
        PointLight* p = &point_lights[i];
        shader_uniform_vec3(g_geometry_shader, light_offset + 0, p->color);
        shader_uniform_float(g_geometry_shader, light_offset + 1, p->ambient_intensity);
        shader_uniform_float(g_geometry_shader, light_offset + 2, p->diffuse_intensity);
        shader_uniform_vec3(g_geometry_shader, light_offset + 3, p->position);
        shader_uniform_float(g_geometry_shader, light_offset + 4, p->atten.constant);
        shader_uniform_float(g_geometry_shader, light_offset + 5, p->atten.linear);
        shader_uniform_float(g_geometry_shader, light_offset + 6, p->atten.exp);

        light_offset += 7;
    }

    mesh_render(mesh);

    g_drawcalls++;
}

void renderer_draw_mesh_shadow(Mesh* mesh, Transform* transform, Mat4* mvp)
{
    shader_bind(g_shadow_shader);
    
    Mat4 mvp_t = mat4_mul2(mvp, &transform->matrix);
    mvp_t = mat4_transposed(&mvp_t);

    shader_uniform_mat4(g_shadow_shader, 0, &mvp_t, 1, false);

    mesh_render(mesh);

    g_drawcalls++;
}

void renderer_draw_mesh_instanced(Mesh* mesh, Texture* texture, Transform* transforms, u32 num_transforms, Camera* cam, SunLight* light)
{
    shader_bind(g_geometry_instanced_shader);
    
    if(texture)
    {
        texture_bind(texture, 0);
        shader_uniform_int(g_geometry_instanced_shader, 9, 1);//use texture
    }
    else
    {
        shader_uniform_int(g_geometry_instanced_shader, 9, 0);//use texture
    }
    
    Mat4* mvp_mats = malloc(sizeof(Mat4) * num_transforms);
    Mat4* world_mats = malloc(sizeof(Mat4) * num_transforms);
    u32 num_instances = num_transforms;
    for(u32 i = 0; i < num_instances; ++i)
    {
        Mat4* transform = &transforms[i].matrix;
        Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
        Mat4 world = mat4_transposed(transform);
        mvp = mat4_mul2(&mvp, transform);
        mvp = mat4_transposed(&mvp);
        mvp_mats[i] = mvp;
        world_mats[i] = world;
    }
    
    if(light)
    {
        shader_uniform_int(g_geometry_instanced_shader,7,1); //use light
        shader_uniform_vec3(g_geometry_instanced_shader,2,light->direction);
        shader_uniform_vec3(g_geometry_instanced_shader,3,light->color);
        shader_uniform_float(g_geometry_instanced_shader,4,light->intensity);
        shader_uniform_vec3(g_geometry_instanced_shader,6,cam->position);
    }
    else
    {
        shader_uniform_int(g_geometry_instanced_shader, 7, 0); //use light
    }
    
    mesh_render(mesh, mvp_mats, world_mats, num_instances);
    
    free(mvp_mats);
    free(world_mats);
    
    g_drawcalls++;
}

void renderer_draw_pick_mesh(PickMesh *mesh, Transform* transform, Camera *cam)
{
    shader_bind(g_picking_shader);
    
    if(transform)
    {
        Mat4 mvp = mat4_mul3(&cam->projection, &cam->view, &transform->matrix);
        mvp = mat4_transposed(&mvp);
        shader_uniform_mat4(g_picking_shader, 0,&mvp,1,false);
    }
    else
    {
        Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
        mvp = mat4_transposed(&mvp);
        shader_uniform_mat4(g_picking_shader, 0,&mvp,1,false);
    }
    
    pick_mesh_render(mesh);
}

void renderer_draw_pick_mesh_instanced(PickMesh* mesh, Transform* transforms, u32 num_transforms, Vec3* colors, Camera* cam)
{
    shader_bind(g_picking_instanced_shader);
    
    Mat4* mvp_mats = malloc(sizeof(Mat4) * num_transforms);
    Mat4* world_mats = malloc(sizeof(Mat4) * num_transforms);
    u32 num_instances = num_transforms;
    for(u32 i = 0; i < num_instances; ++i)
    {
        Mat4* transform = &transforms[i].matrix;
        Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
        Mat4 world = mat4_transposed(transform);
        mvp = mat4_mul2(&mvp, transform);
        mvp = mat4_transposed(&mvp);
        mvp_mats[i] = mvp;
        world_mats[i] = world;
    }
    
    pick_mesh_render_instanced(mesh,mvp_mats, world_mats, colors, num_instances);
    
    free(mvp_mats);
    free(world_mats);
    
    g_drawcalls++;
}

void renderer_draw_terrain(Terrain* terrain, Camera* cam, SunLight* light)
{
    shader_bind(g_terrain_shader);
    
    Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
    mvp = mat4_transposed(&mvp);
    Mat4 tmp;
    mat4_init(&tmp);
    Mat4 world = mat4_transposed(&tmp);
    
    shader_uniform_mat4(g_terrain_shader, 0, &mvp, 1, false);
    shader_uniform_mat4(g_terrain_shader, 1, &world, 1, false);
    
    if(light)
    {
        shader_uniform_int(g_terrain_shader,7,1); //use light
        shader_uniform_vec3(g_terrain_shader,2,light->direction);
        shader_uniform_vec3(g_terrain_shader,3,light->color);
        shader_uniform_float(g_terrain_shader,4,light->intensity);        
        shader_uniform_vec3(g_terrain_shader,6,cam->position);
    }
    else
    {
        shader_uniform_int(g_terrain_shader,7, 0); //use light
    }
    
    terrain_render(terrain);
    g_drawcalls++;
}

void renderer_draw_shape(Shape* shape, Mat4* transform, Camera* cam)
{
    shader_bind(g_shape_shader);
    
    Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
    mvp = mat4_transposed(&mvp);
    
    shader_uniform_mat4(g_shape_shader, 0, &mvp, 1, false);
    
    shape_render(shape);
    g_drawcalls++;
}

void renderer_draw_water(Water* water, Camera *cam)
{
    shader_bind(g_water_shader);
    
    Mat4 mvp = mat4_mul2(&cam->projection, &cam->view);
    mvp = mat4_transposed(&mvp);
    
    shader_uniform_mat4(g_water_shader, 0, &mvp, 1, false);
    
    water_render(water);
    g_drawcalls++;
}

static Rect get_target_rect(u32 win_width, u32 win_height, u32 render_width, u32 render_height)
{
    Rect result;
    
    if(win_width == render_width && win_height == render_height)
    {
        float x, y, w, h;
        x = 0;
        y = 0;
        w = (float)win_width;
        h = (float)win_height;
        result = rect_create(x, y, w, h);
        return result;
    }
    
    float ar = (float)win_width / win_height;
    float st_ar = (float)render_width / render_height;
    
    // screen wider than 16:9
    if(ar > st_ar)
    {
        float new_width = win_height * st_ar;
        float x, y, w, h;
        
        x = (win_width - new_width) / 2;
        w = new_width;
        y = 0;
        h = (float)win_height;
        result = rect_create(x, y, w, h);
    }
    else
    {
        float x, y, w, h;
        float new_height = win_width / st_ar;
        x = 0;
        w = (float)win_width;
        
        y = (win_height - new_height) / 2;
        h = new_height;
        result = rect_create(x, y, w, h);
    }
    
    return result;
}
