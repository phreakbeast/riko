#include "tek_vec2.h"

#include "tek_math_functions.h"

Vec2 vec2_create(float x, float y)
{
    Vec2 res;
    res.x = x;
    res.y = y;
    return res;
}

const float vec2_length(Vec2 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

const Vec2 vec2_normalized(Vec2 vec)
{
    float len = vec2_length(vec);
    if (len == 0) {
        len = 0.0000001f;
    }

    Vec2 res = {
            vec.x / len,
            vec.y / len
    };
    return res;
}

const float vec2_dot(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

const float vec2_distance(Vec2 v1, Vec2 v2)
{
    float a = v1.x - v2.x;
    float b = v1.y - v2.y;
    return sqrtf(a * a + b * b);
}

const Vec2 vec2_add(Vec2 v1, Vec2 v2)
{
    Vec2 res = {
            v1.x + v2.x,
            v1.y + v2.y
    };
    return res;
}

const Vec2 vec2_sub(Vec2 v1, Vec2 v2)
{
    Vec2 res = {
            v1.x - v2.x,
            v1.y - v2.y
    };
    return res;
}

const Vec2 vec2_mul(Vec2 v, float num)
{
    Vec2 res = {
            v.x * num,
            v.y * num
    };
    return res;
}

const Vec2 math_get_point_on_circle(float rad, float angle) 
{
    float x = rad * cosf(math_to_radian(angle));
    float y = rad * sinf(math_to_radian(angle));

    Vec2 res = {x, y};
    return res;
}

