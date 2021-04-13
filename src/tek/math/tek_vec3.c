#include "tek_vec3.h"

#include "tek_math_functions.h"

Vec3 vec3_create(float x, float y, float z)
{
    Vec3 res;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
}

Vec3 math_calc_normals(Vec3 p0, Vec3 p1, Vec3 p2) 
{
    Vec3 v1 = vec3_sub(p1, p0);
    Vec3 v2 = vec3_sub(p2, p0);
    Vec3 normal = vec3_cross(v1,v2);
    normal = vec3_normalized(normal);

    return normal;
}

const float vec3_length(Vec3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

const Vec3 vec3_normalized(Vec3 vec)
{
    float len = vec3_length(vec);
    if (len == 0) {
        len = 0.0000001f;
    }

    Vec3 res = {
            vec.x / len,
            vec.y / len,
            vec.z / len
    };
    return res;
}

const Vec3 vec3_negate(Vec3 vec)
{
    Vec3 res = {-vec.x, -vec.y, -vec.z};
    return res;
}

const float vec3_dot(Vec3 v1, Vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const Vec3 vec3_cross(Vec3 v1, Vec3 v2)
{
    Vec3 res = {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
    };

    return res;
}

const float vec3_distance(Vec3 v1, Vec3 v2)
{
    float a = v1.x - v2.x;
    float b = v1.y - v2.y;
    float c = v1.z - v2.z;
    return sqrtf(a * a + b * b + c * c);
}

const Vec3 vec3_lerp(Vec3 v1, Vec3 v2, float t)
{
    Vec3 res = {
            v1.x + t * (v2.x - v1.x),
            v1.y + t * (v2.y - v1.y),
            v1.z + t * (v2.z - v1.z)
    };
    return res;
}

const Vec3 vec3_add(Vec3 v1, Vec3 v2)
{
    Vec3 res = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
    };
    return res;
}

const Vec3 vec3_sub(Vec3 v1, Vec3 v2)
{
    Vec3 res = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
    };
    return res;
}

const Vec3 vec3_mul(Vec3 v1, float num)
{
    Vec3 res = {
            v1.x * num,
            v1.y * num,
            v1.z * num
    };
    return res;
}