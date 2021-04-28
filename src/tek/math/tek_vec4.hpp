#ifndef _TEK_VEC4_HPP
#define _TEK_VEC4_HPP

#include "tek_vec3.hpp"

typedef struct vec4_t
{
    float x;
    float y;
    float z;
    float w;    
}Vec4;

Vec4 vec4_create(float x, float y, float z, float w);

const float vec4_length(Vec4 vec);
const Vec4 vec4_normalized(Vec4 vec);
const Vec4 vec4_lerp(Vec4 v1, Vec4 v2, float t);

const Vec4 vec4_add(Vec4 v1, Vec4 v2);
const Vec4 vec4_sub(Vec4 v1, Vec4 v2);
const Vec4 vec4_mul(Vec4 v1, float num);

#endif
