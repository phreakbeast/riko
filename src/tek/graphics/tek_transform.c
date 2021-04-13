#include "tek_transform.h"

Transform transform_create()
{
    Transform res;
    res.position = vec3_create(0, 0, 0);
    res.rotation = vec3_create(0, 0, 0);
    res.size = vec3_create(1,1,1);
    transform_calc(&res);
    return res;
}

void transform_init(Transform* transform, Vec3 position, Vec3 rotation, Vec3 size)
{
    transform->position = position;
    transform->rotation = rotation;
    transform->size = size;
    transform_calc(transform);
}

void transform_calc(Transform* trans)
{
    Mat4 translation = mat4_translate(trans->position.x, trans->position.y, trans->position.z);
    Mat4 rot_x = mat4_rotate(trans->rotation.x, 1, 0, 0);
    Mat4 rot_y = mat4_rotate(trans->rotation.y, 0, 1, 0);
    Mat4 rot_z = mat4_rotate(trans->rotation.z, 0, 0, 1);
    Mat4 rot = mat4_mul3(&rot_y, &rot_x, &rot_z);
    Mat4 scale = mat4_scale(trans->size.x, trans->size.y, trans->size.z);

    trans->matrix = mat4_mul3(&translation, &rot, &scale);
}
