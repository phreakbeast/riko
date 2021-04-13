#include "tek_collision.h"

#include <stdio.h>

static float sign(Vec2 p1, Vec2 p2, Vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool point_in_triangle(Vec2 pt, Vec2 v1, Vec2 v2, Vec2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;
    
    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);
    
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

static bool _ray_sphere_collision(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r,
                           float* dist, Vec3* point)
{
    float b = 2 * (xd * (xs - xc) + yd * (ys - yc) + zd * (zs - zc));
    float c = xs * xs - 2 * xs * xc + xc * xc + ys * ys - 2 * ys * yc + yc * yc + zs * zs - 2 * zs * zc + zc * zc - r * r;
    float disc = (b * b - 4 * c);
    if(disc < 0)
    {
        return false;
    }
    
    if(dist != NULL)
    {
        (*dist) = (-b + sqrtf(disc)) / 2;
        if(point != NULL)
        {
            point->x = xs + (*dist) * xd;
            point->y = ys + (*dist) * yd;
            point->z = zs + (*dist) * zd;
        }
    }
    
    return true;
}


static float triangle_area(Vec3 p1, Vec3 p2, Vec3 p3)
{
//length
    float a = sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
    float b = sqrtf((p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y) + (p3.z - p2.z) * (p3.z - p2.z));
    float c = sqrtf((p3.x - p1.x) * (p3.x - p1.x) + (p3.y - p1.y) * (p3.y - p1.y) + (p3.z - p1.z) * (p3.z - p1.z));
    
    float s = (a + b + c) / 2;
    
    return sqrtf(s * (s - a) * (s - b) * (s - c));
}

static bool _ray_plane_collision(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd, Vec3 p1,
                     Vec3 p2, Vec3 p3, Vec3 p4, float* dist, Vec3* point)
{
//dot product between plane and normal
    float a = xd * nx + yd * ny + zd * nz;
    if(a == 0)
    { // parallel
        return false;
    }
    
    float t = ((p1.x * nx + p1.y * ny + p1.z * nz - nx * xs - ny * ys - nz * zs) / a);
    if(t < 0)
    { //negative collision
        return false;
    }

    //collision point
    float x = xs + t * xd;
    float y = ys + t * yd;
    float z = zs + t * zd;
    
    Vec3 cp = vec3_create(x, y, z);
    
    if((fabs(triangle_area(p1, p3, p4)- triangle_area(p1, p4, cp) - triangle_area(p1, p3, cp) - triangle_area(p3, p4, cp)) < 0.001) ||
       (fabs(triangle_area(p1, p2, p3)-triangle_area(p1, p2, cp) - triangle_area(p2, p3, cp) - triangle_area(p1, p3, cp)) < 0.001))
    {
        if(dist != NULL)
        {
            (*dist) = t;
            if(point != NULL)
            {
                point->x = x;
                point->y = y;
                point->z = z;
            }
        }
        return true;
    }
    
    return false;
}


static bool _ray_triangle_collision(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd,
                             Vec3 p1, Vec3 p2, Vec3 p3, float* dist, Vec3* point)
{
//dot product between plane and normal
    float a = xd * nx + yd * ny + zd * nz;
    if(a == 0)
    { // parallel
        return false;
    }
    
    
    float t = ((p1.x * nx + p1.y * ny + p1.z * nz - nx * xs - ny * ys - nz * zs) / a);
    if(t < 0)
    { //negative collision
        return false;
    }

//collision point
    float x = xs + t * xd;
    float y = ys + t * yd;
    float z = zs + t * zd;

//std::cout << x << " " << y << " " << z << std::endl;
    
    Vec3 cp = vec3_create(x, y, z);
    
    float f = fabsf(triangle_area(p1, p2, p3) - triangle_area(p1, p2, cp) - triangle_area(p2, p3, cp) - triangle_area(p1, p3, cp));
    //printf("%f\n",f);
    
    if(fabsf(triangle_area(p1, p2, p3) - triangle_area(p1, p2, cp) - triangle_area(p2, p3, cp) - triangle_area(p1, p3, cp)) < 0.001)
    {
        if(dist != NULL)
        {
            (*dist) = t;
            if(point != NULL)
            {
                point->x = x;
                point->y = y;
                point->z = z;
            }
        }
        return true;
    }
    
    return false;
}

float pointdistance(Vec3 c1, Vec3 c2)
{
    Vec3 a = vec3_sub(c2, c1);
    return (a.x * a.x + a.y * a.y + a.z * a.z);
}

static bool _sphere_sphere_collision(Vec3 c1, float r1, Vec3 c2, float r2)
{
    float dist = pointdistance(c1, c2);
    if(dist <= (r1 + r2) * (r1 + r2))
    {
        float a = sqrtf(dist) - (r1 + r2);
        Vec3 vec = vec3_sub(c2, c1);
        vec = vec3_normalized(vec);
        c1 = vec3_mul(vec3_add(c1, vec), a);
        return true;
    }
    return false;
}

static bool _sphere_plane_collision(Vec3 sp, Vec3 vn, Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, float r)
{
    float dist1 = 0, dist2 = 0;
    
    if(_ray_plane_collision(-vn.x, -vn.y, -vn.z, sp.x, sp.y, sp.z, vn.x, vn.y, vn.z, p1, p2, p3, p4, &dist1, NULL) ||
       _ray_plane_collision(vn.x, vn.y, vn.z, sp.x, sp.y, sp.z, -vn.x, -vn.y, -vn.z, p1, p2, p3, p4, &dist2, NULL))
    {
        if(dist1 > r || dist2 > r)
        {
            return false;
        }
        if(dist1 > 0)
        {
            sp.x = sp.x - vn.x * (r - dist1);
            sp.y = sp.y - vn.y * (r - dist1);
            sp.z = sp.z - vn.z * (r - dist1);
        }
        else
        {
            sp.x = sp.x + vn.x * (r - dist2);
            sp.y = sp.y + vn.y * (r - dist2);
            sp.z = sp.z + vn.z * (r - dist2);
        }
        return true;
    }
    
    return false;
}

bool _sphere_triangle_collision(Vec3 sp, Vec3 vn, Vec3 p1, Vec3 p2, Vec3 p3, float r)
{
    float dist1 = 0, dist2 = 0;
    
    if(_ray_triangle_collision(-vn.x, -vn.y, -vn.z, sp.x, sp.y, sp.z, vn.x, vn.y, vn.z, p1, p2, p3, &dist1, NULL) ||
       _ray_triangle_collision(vn.x, vn.y, vn.z, sp.x, sp.y, sp.z, -vn.x, -vn.y, -vn.z, p1, p2, p3, &dist2, NULL))
    {
        if(dist1 > r || dist2 > r)
        {
            return false;
        }
        if(dist1 > 0)
        {
            sp.x = sp.x - vn.x * (r - dist1);
            sp.y = sp.y - vn.y * (r - dist1);
            sp.z = sp.z - vn.z * (r - dist1);
        }
        else
        {
            sp.x = sp.x + vn.x * (r - dist2);
            sp.y = sp.y + vn.y * (r - dist2);
            sp.z = sp.z + vn.z * (r - dist2);
        }
        return true;
    }
    
    return false;
}

bool ray_sphere_collision(CollisionRay* p_ray, CollisionSphere* p_sphere, float* dist, Vec3* point)
{
    return _ray_sphere_collision(p_sphere->position.x, p_sphere->position.y, p_sphere->position.z,
                                 p_ray->direction.x, p_ray->direction.y, p_ray->direction.z, p_ray->position.x,
                                 p_ray->position.y,
                                 p_ray->position.z, p_sphere->radius, dist, point);
}

bool sphere_sphere_collision(CollisionSphere* p_s1, CollisionSphere* p_s2)
{
    return _sphere_sphere_collision(p_s1->position, p_s1->radius, p_s2->position, p_s2->radius);
}

bool sphere_plane_collision(CollisionSphere* p_sphere, CollisionPlane* p_plane)
{
    return _sphere_plane_collision(p_sphere->position, p_plane->normal, p_plane->p1, p_plane->p2, p_plane->p3, p_plane->p4, p_sphere->radius);
}

bool sphere_triangle_collision(CollisionSphere* p_sphere, CollisionTriangle* p_triangle)
{
    return _sphere_triangle_collision(p_sphere->position, p_triangle->normal, p_triangle->p1, p_triangle->p2, p_triangle->p3, p_sphere->radius);
}

bool ray_triangle_collision(CollisionRay* p_ray, CollisionTriangle* p_triangle, Vec3* p_point, float* p_dist)
{
    return _ray_triangle_collision(p_triangle->normal.x, p_triangle->normal.y, p_triangle->normal.z, p_ray->position.x,
                                   p_ray->position.y, p_ray->position.z, p_ray->direction.x,
                                   p_ray->direction.y, p_ray->direction.z, p_triangle->p1, p_triangle->p2,
                                   p_triangle->p3, p_dist, p_point);
}

bool ray_plane_collision(CollisionRay* p_ray, CollisionPlane* p_plane, float* dist, Vec3* point)
{
    return _ray_plane_collision(p_plane->normal.x, p_plane->normal.y, p_plane->normal.z, p_ray->position.x,
                                p_ray->position.y, p_ray->position.z, p_ray->direction.x,
                                p_ray->direction.y, p_ray->direction.z, p_plane->p1, p_plane->p2, p_plane->p3,
                                p_plane->p4, dist, point);
}


