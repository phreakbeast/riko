#ifndef _TEK_CAMERA_HPP
#define _TEK_CAMERA_HPP

#include "../math/tek_math.hpp"

typedef struct camera_s
{
    Vec3 position;
    Vec3 rotation;

    Mat4 view;
    Mat4 view_rot;
    Mat4 projection;

    Vec3 target_position;
    Vec3 target_rotation;

    float distance;
    float min_distance;
    float max_distance;
    float min_tilt;
    float max_tilt;
}Camera;

Camera cam_create(Vec3 position, Mat4 projection);

void cam_calc(Camera* cam);
void cam_move(Camera* cam, float dir, float speed);
void cam_move_up(Camera* cam, float dir, float speed);
void cam_rotate_x(Camera* cam, float val);
void cam_rotate_y(Camera* cam, float val);
void cam_zoom(Camera* cam, float val);

#endif
