#ifndef _TEK_ANIMATION_HPP
#define _TEK_ANIMATION_HPP

#include "../math/tek_math.hpp"
#include "../utils/tek_core.hpp"

typedef struct
{
    u32 vbo;
}frame_t;

typedef struct
{
    u32 num_vertices;
    u32 num_indices;
    u32 vao;
    u32 ibo;
    frame_t* frames;
    u32 num_frames;
}animation_t;

animation_t* anim_load(const char* filename);
void anim_destroy(animation_t* anim);
void anim_render(animation_t* anim, u32 frame);

#endif
