#include "tek_collision.h"

#include "../graphics/tek_shapebuffer.h"
#include <stddef.h>

void collision_mesh_init(CollisionMesh* mesh)
{
    mesh->triangles = NULL;
    mesh->num_triangles = 0;
}

void collision_mesh_destroy(CollisionMesh* mesh)
{
    free(mesh->triangles);
}

void collision_triangle_calculate_normal(CollisionTriangle* tri)
{
    Vec3 u = vec3_sub(tri->p2, tri->p1);
    Vec3 v = vec3_sub(tri->p3, tri->p1);
    
    tri->normal = vec3_cross(u,v);
    tri->normal = vec3_normalized(tri->normal);
}

void collision_plane_calculate_normal(CollisionPlane* plane)
{
    Vec3 u = vec3_sub(plane->p2 ,plane->p1);
    Vec3 v = vec3_sub(plane->p3 ,plane->p1);
    
    plane->normal = vec3_cross(u,v);
    plane->normal = vec3_normalized(plane->normal);
}

static void collision_object_init(CollisionObject* obj)
{
    obj->sphere = NULL;
    obj->ray = NULL;
    obj->plane = NULL;
    obj->triangle = NULL;
    obj->mesh = NULL;
}

void collision_object_destroy(CollisionObject* obj)
{
    free(obj->sphere);
    free(obj->ray);
    free(obj->plane);
    free(obj->triangle);
    free(obj->mesh);
}

CollisionObject* collision_object_create_sphere(Vec3 position, float radius)
{
    CollisionObject* obj = malloc(sizeof(CollisionObject));
    collision_mesh_init(obj);

    obj->type = COLLISION_OBJECT_SPHERE;
    
    obj->sphere = malloc(sizeof(CollisionSphere));
    obj->sphere->position = position;
    obj->sphere->radius = radius;
    
    return obj;
}

CollisionObject* collision_object_create_ray(Vec3 position, Vec3 direction)
{
    CollisionObject* obj = malloc(sizeof(CollisionObject));
    collision_mesh_init(obj);
    
    obj->type = COLLISION_OBJECT_RAY;
    obj->ray = malloc(sizeof(CollisionRay));
    
    obj->ray->position = position;
    obj->ray->direction = direction;
    
    return obj;
}

CollisionObject* collision_object_create_plane(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3)
{
    CollisionObject* obj = malloc(sizeof(CollisionObject));
    collision_mesh_init(obj);
    
    obj->type = COLLISION_OBJECT_PLANE;
    
    obj->plane = malloc(sizeof(CollisionPlane));
    obj->plane->p1 = p0;
    obj->plane->p2 = p1;
    obj->plane->p3 = p2;
    obj->plane->p4 = p3;
    collision_plane_calculate_normal(obj->plane);
    
    return obj;
}

CollisionObject* collision_object_create_triangle(Vec3 p0, Vec3 p1, Vec3 p2)
{
    CollisionObject* obj = malloc(sizeof(CollisionObject));
    collision_mesh_init(obj);
    
    obj->type = COLLISION_OBJECT_TRIANGLE;
    obj->triangle = malloc(sizeof(CollisionTriangle));
    
    obj->triangle->p1 = p0;
    obj->triangle->p2 = p1;
    obj->triangle->p3 = p2;
    
    collision_triangle_calculate_normal(obj->triangle);
    
    return obj;
}

CollisionObject* collision_object_create_mesh(CollisionTriangle* triangles, u32 num_triangles)
{
    CollisionObject* obj = malloc(sizeof(CollisionObject));
    collision_mesh_init(obj);
    
    obj->type = COLLISION_OBJECT_MESH;
    obj->mesh = malloc(sizeof(CollisionMesh));
    
    obj->mesh->num_triangles = num_triangles;
    obj->mesh->triangles = malloc(sizeof(CollisionTriangle) * num_triangles);
    memcpy(obj->mesh->triangles,triangles,sizeof(CollisionTriangle) * num_triangles);
    
    return obj;
}
