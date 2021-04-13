#ifndef _G_MAP_H
#define _G_MAP_H

#include "../tek/graphics/tek_mesh.h"
#include "../tek/graphics/tek_material.h"
#include "../tek/graphics/tek_transform.h"
#include "../tek/graphics/tek_camera.h"
#include "../tek/graphics/tek_terrain.h"
#include "../tek/graphics/tek_shape.h"
#include "../tek/graphics/tek_picking.h"
#include "../tek/graphics/tek_light.h"
#include "../tek/graphics/tek_water.h"
#include "../tek/math/tek_collision.h"

typedef enum map_object_type_e
{
    MAP_OBJ_STATIC_MESH,
    MAP_OBJ_BILLBOARD,
    MAP_OBJ_PARTICLE
}MapObjectType;

typedef struct map_static_mesh
{
    u32 mesh_id;
    u32 mat_id;
}MapStaticMesh;

typedef struct map_object_s
{
    MapObjectType type;
    MapStaticMesh static_mesh;
    
    Transform transform;
}MapObject;

typedef struct map_s
{            
    Mesh** meshes;
    u32 num_meshes;

    Material** materials;
    u32 num_materials;

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
