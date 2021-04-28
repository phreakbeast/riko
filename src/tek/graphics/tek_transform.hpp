#ifndef _TEK_TRANSFORM_HPP
#define _TEK_TRANSFORM_HPP

#include "../math/tek_vec3.hpp"
#include "../math/tek_mat4.hpp"

typedef struct transform_s
{
    Vec3 position;
    Vec3 rotation;
    Vec3 size;
    Mat4 matrix;    
}Transform;

Transform transform_create();
void transform_init(Transform* transform, Vec3 position, Vec3 rotation, Vec3 size);

void transform_calc(Transform* trans);

#endif
