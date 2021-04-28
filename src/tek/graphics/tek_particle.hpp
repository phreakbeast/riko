#ifndef _TEK_PARTICLE_HPP
#define _TEK_PARTICLE_HPP

#include "../math/tek_math.hpp"
#include "tek_color.hpp"
#include "tek_texture.hpp"
#include "tek_rect.hpp"


typedef enum emitter_type_e
{
    EMITTER_POINT,
    EMITTER_LINE,
    EMITTER_CIRCLE,
    EMITTER_SPHERE,
    EMITTER_SQUARE,
    EMITTER_BOX
}EmitterType;

typedef struct particle_emitter_s
{
    EmitterType type;
    Vec3 origin;
    Vec3 direction;
    Vec3 line_start;
    Vec3 line_end;
    float radius;
    float width;
    float depth;
    float height;
}ParticleEmitter;

Vec3 emitter_get_point(ParticleEmitter* emitter, bool inside);

ParticleEmitter emitter_create(EmitterType type);

Vec3 emitter_get_random_point(ParticleEmitter *emitter);

typedef struct particle_vertex_data_s
{
    Vec3 position;
    Vec4 color;
    Vec4 uv_rect;
    float size;
    float life;
    Vec3 direction;
    float dist_to_cam;
}ParticleVertexData;

//for sorting
/*
bool operator<(ParticleVertexData &p1)
{
    return this->dist_to_cam > p1.dist_to_cam;
}
 */

typedef struct particle_system_s
{
    Texture *texture;

    ParticleVertexData* vertices;
    u32 num_vertices;

    ParticleEmitter emitter;

    u32 num_particles;

    Vec4 color_start;
    Vec4 color_end;
    float size_start;
    float size_end;

    Vec3 spread;
    float speed;
    float gravity;
    float life;

    Rect *uvs;
    u32 num_uvs;

    u32 vao;
    u32 vbo;
}ParticleSystem;

void particle_destroy(ParticleSystem* ps);

void particle_update(ParticleSystem* ps, float delta, Vec3 camera_pos);

void particle_render(ParticleSystem* ps);

ParticleSystem* particle_load(const char *filename);

#endif
