#ifndef _TEK_COLLISION_HPP
#define _TEK_COLLISION_HPP

#include "../utils/tek_core.hpp"
#include "tek_math.hpp"
#include "../graphics/tek_color.hpp"

//2d
bool point_in_triangle(Vec2 pt, Vec2 v1, Vec2 v2, Vec2 v3);

//3d
typedef struct collision_sphere_t
{
    Vec3 position;
    float radius;
}CollisionSphere;

typedef struct collision_ray_t
{
    Vec3 position;
    Vec3 direction;
}CollisionRay;

typedef struct collision_plane_t
{
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    Vec3 p4;
    
    Vec3 normal;        
}CollisionPlane;
void collision_plane_calculate_normal(CollisionPlane* plane);

typedef struct collision_triangle_t
{
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    
    Vec3 normal;        
}CollisionTriangle;

void collision_triangle_calculate_normal(CollisionTriangle* tri);

typedef struct collision_mesh_t
{        
    CollisionTriangle* triangles;
    u32 num_triangles;
}CollisionMesh;

void collision_mesh_init(CollisionMesh* mesh);
void collision_mesh_destroy(CollisionMesh* mesh);

typedef enum
{
    COLLISION_OBJECT_SPHERE,
    COLLISION_OBJECT_RAY,
    COLLISION_OBJECT_PLANE,
    COLLISION_OBJECT_TRIANGLE,
    COLLISION_OBJECT_MESH,
}CollisionObjectType;

typedef struct collision_object_s
{        
    CollisionObjectType type;    
    
    CollisionSphere* sphere;
    CollisionRay* ray;
    CollisionPlane* plane;
    CollisionTriangle* triangle;
    CollisionMesh* mesh;
}CollisionObject;

void collision_object_destroy(CollisionObject* obj);

void collision_object_create_sphere(CollisionObject* obj, Vec3 position, float radius);
void collision_object_create_ray(CollisionObject* obj, Vec3 position, Vec3 direction);
void collision_object_create_plane(CollisionObject* obj, Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3);
void collision_object_create_triangle(CollisionObject* obj, Vec3 p0, Vec3 p1, Vec3 p2);
void collision_object_create_mesh(CollisionObject* obj, CollisionTriangle* triangles, u32 num_triangles);


bool ray_plane_collision(CollisionRay* p_ray, CollisionPlane* p_plane, float* dist, Vec3* point);
bool ray_sphere_collision(CollisionRay* p_ray, CollisionSphere* p_sphere, float* dist, Vec3* point);
bool ray_triangle_collision(CollisionRay* p_ray, CollisionTriangle* p_triangle, Vec3* p_point, float* p_dist);
bool sphere_sphere_collision(CollisionSphere* p_s1, CollisionSphere* p_s2);
bool sphere_plane_collision(CollisionSphere* p_sphere, CollisionPlane* p_plane);
bool sphere_triangle_collision(CollisionSphere* p_sphere, CollisionTriangle* p_triangle);

#endif
