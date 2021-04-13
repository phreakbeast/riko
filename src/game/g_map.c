#include "g_map.h"

#include "../tek/graphics/tek_drawing.h"
#include "../tek/graphics/tek_geometry.h"
#include <stdlib.h>
#include <assert.h>

void map_destroy(Map* map)
{
    for (u32 i = 0; i < map->num_meshes; ++i)
    {
        mesh_destroy(map->meshes[i]);
    }
    free(map->meshes);

    for (u32 i = 0; i < map->num_materials; ++i)
    {
        material_destroy(map->materials[i]);
    }
    free(map->materials);

    free(map->objects);

    free(map->point_lights);
}

void map_update(Map* map, float delta, Camera* cam)
{
    for (u32 i = 0; i < map->num_objects; ++i)
    {        
        MapObject* obj = &map->objects[i];
        switch (obj->type)
        {
        case MAP_OBJ_STATIC_MESH:
        {
            Mesh* mesh = map->meshes[obj->static_mesh.mesh_id];
            Material* mat = map->materials[obj->static_mesh.mat_id];

            renderer_draw_mesh(mesh, mat, &obj->transform, cam, &map->sun_light,map->point_lights,map->num_point_lights);
        }break;
        default:
            break;
        }
    }
}

void map_render_shadows(Map* map, Camera* cam)
{
    Mat4 projection = mat4_perspective(90.0f, 512.0f / 512.0f, 0.1f, 100.0f);
    Mat4 shadow = shadow_matrix(vec3_create(20, 40, 0), vec3_create(80,0,0), projection);
    for (u32 i = 0; i < map->num_objects; ++i)
    {
        MapObject* obj = &map->objects[i];
        switch (obj->type)
        {
        case MAP_OBJ_STATIC_MESH:
        {
            Mesh* mesh = map->meshes[obj->static_mesh.mesh_id];            

            renderer_draw_mesh_shadow(mesh, &obj->transform, &shadow);
        }break;
        default:
            break;
        }
    }
}

Map* map_load()
{
    Map* map = malloc(sizeof(Map));
    assert(map);

    map->num_materials = 2;
    map->materials = malloc(sizeof(Material*) * map->num_materials);
    assert(map->materials);

    map->materials[0] = material_create();
    map->materials[0]->diffuse_map = texture_load("data/textures/stone_floor.tga", TEXTURE_FILTER_LINEAR);
    map->materials[0]->specular_map = NULL;

    map->materials[1] = material_create();
    map->materials[1]->diffuse_map = texture_load("data/textures/floor.tga", TEXTURE_FILTER_LINEAR);
    map->materials[1]->specular_map = NULL;

    map->num_meshes = 3;
    map->meshes = malloc(sizeof(Mesh*) * map->num_meshes);
    assert(map->meshes);

    MeshBuffer buffer;
    mesh_buffer_init(&buffer);
    //mesh_buffer_add_sphere(&buffer, 3, 16, 16, 0, color_white());
    if (!mesh_buffer_load_mesh(&buffer, "data/meshes/female.obj")) return false;

    map->meshes[0] = mesh_buffer_to_mesh(&buffer);
    mesh_buffer_destroy(&buffer);

    MeshBuffer buffer2;
    mesh_buffer_init(&buffer2);
    mesh_buffer_add_plane(&buffer2, 30, 30, color_white());    

    map->meshes[1] = mesh_buffer_to_mesh(&buffer2);
    mesh_buffer_destroy(&buffer2);

    MeshBuffer buffer3;
    mesh_buffer_init(&buffer3);
    mesh_buffer_add_box(&buffer3, 5, 5, 5, color_white(),vec3_create(0,0,0));

    map->meshes[2] = mesh_buffer_to_mesh(&buffer3);
    mesh_buffer_destroy(&buffer3);

    map->num_objects = 3;
    map->objects = malloc(sizeof(MapObject) * map->num_objects);
    assert(map->objects);

    MapObject* obj1 = &map->objects[0];
    obj1->type = MAP_OBJ_STATIC_MESH;
    obj1->static_mesh.mesh_id = 0;
    obj1->static_mesh.mat_id = 0;    
    transform_init(&obj1->transform, vec3_create(0,0,0), vec3_create(0,0,0), vec3_create(1,1,1));
    
    MapObject* obj2 = &map->objects[1];
    obj2->type = MAP_OBJ_STATIC_MESH;
    obj2->static_mesh.mesh_id = 1;
    obj2->static_mesh.mat_id = 1;
    transform_init(&obj2->transform, vec3_create(0, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

    MapObject* obj3 = &map->objects[2];
    obj3->type = MAP_OBJ_STATIC_MESH;
    obj3->static_mesh.mesh_id = 2;
    obj3->static_mesh.mat_id = 0;
    transform_init(&obj3->transform, vec3_create(10, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

    map->sun_light.color = vec3_create(0.8f, 0.8f, 0.8f);
    map->sun_light.direction = vec3_create(0.0f, -1.0f, -0.5f);
    map->sun_light.intensity = 0.9f;

    map->num_point_lights = 3;
    map->point_lights = malloc(sizeof(PointLight) * map->num_point_lights);
    assert(map->point_lights);

    map->point_lights[0].color = vec3_create(0, 0, 1);
    map->point_lights[0].position = vec3_create(0, 3, -3);
    map->point_lights[0].ambient_intensity = 0.8f;
    map->point_lights[0].diffuse_intensity = 1.0f;
    map->point_lights[0].atten.constant = 1;
    map->point_lights[0].atten.linear = 0.05;
    map->point_lights[0].atten.exp = 0.05;

    map->point_lights[1].color = vec3_create(0, 1, 0);
    map->point_lights[1].position = vec3_create(3, 3, 3);
    map->point_lights[1].ambient_intensity = 0.8f;
    map->point_lights[1].diffuse_intensity = 1.0f;
    map->point_lights[1].atten.constant = 1;
    map->point_lights[1].atten.linear = 0.05;
    map->point_lights[1].atten.exp = 0.05;

    map->point_lights[2].color = vec3_create(1, 0, 0);
    map->point_lights[2].position = vec3_create(-3, 3, 0);
    map->point_lights[2].ambient_intensity = 0.8f;
    map->point_lights[2].diffuse_intensity = 1.0f;
    map->point_lights[2].atten.constant = 1;
    map->point_lights[2].atten.linear = 0.05;
    map->point_lights[2].atten.exp = 0.05;

    return map;
}
