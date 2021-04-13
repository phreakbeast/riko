#include "tek_vec4.h"

#include "tek_math_functions.h"

Vec4 vec4_create(float x, float y, float z, float w)
{
    Vec4 res;
    res.x = x;
    res.y = y;
    res.z = z;
    res.w = w;
    return res;
}

const float vec4_length(Vec4 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w + vec.w);
}

const Vec4 vec4_normalized(Vec4 vec)
{
    float len = vec4_length(vec);
    if (len == 0) {
        len = 0.0000001f;
    }

    Vec4 res = {
            vec.x / len,
            vec.y / len,
            vec.z / len,
            vec.w / len
    };
    return res;
}

const Vec4 vec4_lerp(Vec4 v1, Vec4 v2, float t)
{
    Vec4 res = {
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z),
            v1.w + t * (v2.w - v1.w)
    };
    return res;
}

const Vec4 vec4_add(Vec4 v1, Vec4 v2)
{
    Vec4 res = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w
    };
    return res;
}

const Vec4 vec4_sub(Vec4 v1, Vec4 v2)
{
    Vec4 res = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w
    };
    return res;
}

const Vec4 vec4_mul(Vec4 v1, float num)
{
    Vec4 res = {
            v1.x * num,
            v1.y * num,
            v1.z * num,
            v1.w * num
    };
    return res;
}
