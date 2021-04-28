#ifndef _G_MAP_HPP
#define _G_MAP_HPP

#include "../tek/graphics/tek_mesh.hpp"
#include "../tek/graphics/tek_material.hpp"
#include "../tek/graphics/tek_transform.hpp"
#include "../tek/graphics/tek_camera.hpp"
#include "../tek/graphics/tek_terrain.hpp"
#include "../tek/graphics/tek_shape.hpp"
#include "../tek/graphics/tek_picking.hpp"
#include "../tek/graphics/tek_light.hpp"
#include "../tek/graphics/tek_water.hpp"
#include "../tek/graphics/tek_animation.hpp"
#include "../tek/math/tek_collision.hpp"

typedef enum map_object_type_e
{
    MAP_OBJ_STATIC_MESH,
    MAP_OBJ_ANIMATED_MESH,
    MAP_OBJ_BILLBOARD,
    MAP_OBJ_PARTICLE
}MapObjectType;

typedef struct map_static_mesh_s
{
    u32 mesh_id;
    u32 mat_id;
}MapStaticMesh;

typedef struct map_animated_mesh_s
{
    u32 mesh_id;
    u32 mat_id;
    float frame;
}MapAnimatedMesh;

typedef struct map_object_s
{
    MapObjectType type;
    MapStaticMesh static_mesh;
    MapAnimatedMesh animated_mesh;

    bool cast_shadow;
    Transform transform;
    int collision_obj;
}MapObject;

typedef struct map_s
{            
    Mesh** meshes;
    u32 num_meshes;

    Material** materials;
    u32 num_materials;

    animation_t** animations;
    u32 num_animations;

    CollisionObject* collision_objects;
    u32 num_collision_objects;

    MapObject* objects;
    u32 num_objects;

    SunLight sun_light;

    PointLight* point_lights;
    u32 num_point_lights;
}Map;

void map_destroy(Map* map);
void map_update(Map* map, float delta, Camera* cam);
void map_render_shadows(Map* map, Camera* cam);

Map* map_load();

#endif
