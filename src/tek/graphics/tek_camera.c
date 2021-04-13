#include "tek_camera.h"


Camera cam_create(Vec3 position, Mat4 projection)
{
    Camera cam;
    cam.projection = projection;
    cam.position = position;
    
    cam.rotation = vec3_create(40, 0, 0);
    cam.target_position = position;
    cam.target_rotation = vec3_create(0, 0, 0);
    cam.distance = 30;
    cam.min_distance = 10;
    cam.max_distance = 100;
    cam.min_tilt = 15;
    cam.max_tilt = 75;
    
    cam_calc(&cam);

    return cam;
}

void cam_calc(Camera* cam)
{
    //calculate camera view
    //calc distance to target point
    float dist_hor  = cam->distance * cosf(math_to_radian(cam->rotation.x));
    float dist_vert = cam->distance * sinf(math_to_radian(cam->rotation.x));
    
    //calc position
    cam->position.y = cam->target_position.y + dist_vert;
    
    float theta = cam->target_rotation.y;
    float x_offset = dist_hor * sinf(math_to_radian(theta));
    float z_offset = dist_hor * cosf(math_to_radian(theta));
    
    cam->position.x = cam->target_position.x + x_offset;
    cam->position.z = cam->target_position.z + z_offset;
    
    //g_cam.rotation.y = 180 - theta;
    cam->rotation.y = theta;
    
    Mat4 trans = mat4_translate(-cam->position.x, -cam->position.y, -cam->position.z);
    Mat4 rot_x = mat4_rotate(cam->rotation.x, 1, 0, 0);
    Mat4 rot_y = mat4_rotate(-cam->rotation.y, 0, 1, 0);
    Mat4 rot = mat4_mul2(&rot_x, &rot_y);
    
    cam->view = mat4_mul2(&rot, &trans);
    cam->view_rot = rot;
}

void cam_move(Camera* cam, float dir, float speed)
{
    float rad = math_to_radian(cam->target_rotation.y + dir);
    cam->target_position.x -= sinf(rad) * speed;
    cam->target_position.z -= cosf(rad) * speed;
}

void cam_move_up(Camera* cam, float dir, float speed)
{
    float rad = math_to_radian(cam->target_rotation.y + dir);
    cam->target_position.y += sinf(rad) * speed;
}

void cam_rotate_x(Camera* cam, float val)
{
    cam->rotation.x += val;
    if(cam->rotation.x <cam-> min_tilt)
    {
        cam->rotation.x = cam->min_tilt;
    }
    if(cam->rotation.x > cam->max_tilt)
    {
        cam->rotation.x = cam->max_tilt;
    }
}

void cam_rotate_y(Camera* cam, float val)
{
    cam->target_rotation.y += val;
    if(cam->target_rotation.y > 360.0f)
    {
        cam->target_rotation.y -= 360.0f;
    }
    if(cam->target_rotation.y < 0.0f)
    {
        cam->target_rotation.y += 360.0f;
    }
}

void cam_zoom(Camera* cam, float val)
{
    cam->distance += val * 5;
    if(cam->distance > cam->max_distance)
    {
        cam->distance = cam->max_distance;
    }
    
    if(cam->distance < cam->min_distance)
    {
        cam->distance = cam->min_distance;
    }
}
