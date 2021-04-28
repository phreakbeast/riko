#ifndef _TEK_VEC2_HPP
#define _TEK_VEC2_HPP

typedef struct vec2_t
{
    float x;
    float y;
}Vec2;

Vec2 vec2_create(float x, float y);

const float vec2_length(Vec2 vec);

const Vec2 vec2_normalized(Vec2 vec);

const float vec2_dot(Vec2 v1, Vec2 v2);

const float vec2_distance(Vec2 v1, Vec2 v2);

const Vec2 vec2_add(Vec2 v1, Vec2 v2);
const Vec2 vec2_sub(Vec2 v1, Vec2 v2);
const Vec2 vec2_mul(Vec2 v, float num);


const Vec2 math_get_point_on_circle(float rad, float angle);

#endif
