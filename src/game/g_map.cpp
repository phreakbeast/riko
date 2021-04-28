#include "g_map.hpp"

#include "../tek/graphics/tek_drawing.hpp"
#include "../tek/graphics/tek_geometry.hpp"
#include "../tek/platform/tek_input.hpp"
#include <stdlib.h>
#include <assert.h>

static Camera g_shadow_cam;

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

    for (u32 i = 0; i < map->num_animations; ++i)
    {
        anim_destroy(map->animations[i]);
    }
    free(map->animations);

    for (u32 i = 0; i < map->num_collision_objects; ++i)
    {
        collision_object_destroy(&map->collision_objects[i]);
    }
    free(map->collision_objects);

    free(map->objects);

    free(map->point_lights);
}

void map_update(Map* map, float delta, Camera* cam)
{
    KeyboardState* kstate = input_get_key_state();
    if(kstate->keys_down[KEY_LEFT])
    {
        cam_move(&g_shadow_cam,90,delta*10);
    }
    if(kstate->keys_down[KEY_RIGHT])
    {
        cam_move(&g_shadow_cam,270,delta*10);
    }
    if(kstate->keys_down[KEY_UP])
    {
        cam_rotate_y(&g_shadow_cam, delta * 50);
    }
    if(kstate->keys_down[KEY_DOWN])
    {
        cam_rotate_y(&g_shadow_cam, -delta * 50);
    }
    cam_calc(&g_shadow_cam);

    Mat4 shd = mat4_mul2(&g_shadow_cam.projection,&g_shadow_cam.view);
    for (u32 i = 0; i < map->num_objects; ++i)
    {        
        MapObject* obj = &map->objects[i];
        switch (obj->type)
        {
        case MAP_OBJ_STATIC_MESH:
        {
            Mesh* mesh = map->meshes[obj->static_mesh.mesh_id];
            Material* mat = map->materials[obj->static_mesh.mat_id];
            Mat4 light_mvp = mat4_mul2(&shd, &obj->transform.matrix);

            renderer_draw_mesh(mesh, mat, &obj->transform, cam, &map->sun_light,map->point_lights,map->num_point_lights, &light_mvp);
        }break;
        case MAP_OBJ_ANIMATED_MESH:
        {
            animation_t* anim = map->animations[obj->animated_mesh.mesh_id];

            //update animation timer
            obj->animated_mesh.frame += delta;
            if(obj->animated_mesh.frame >= anim->num_frames)
            {
                obj->animated_mesh.frame = 0;
            }

            //render mesh
            Material* mat = map->materials[obj->animated_mesh.mat_id];

            renderer_draw_animation(anim, mat, &obj->transform, cam, obj->animated_mesh.frame, &map->sun_light,map->point_lights,map->num_point_lights);
        }break;
        default:
            break;
        }
    }
}

void map_render_shadows(Map* map, Camera* cam)
{
    //Mat4 projection = mat4_perspective(120.0f, 512.0f / 512.0f, 0.1f, 100.0f);
    Mat4 shd = mat4_mul2(&g_shadow_cam.projection,&g_shadow_cam.view);
    for (u32 i = 0; i < map->num_objects; ++i)
    {
        MapObject* obj = &map->objects[i];
        if(!obj->cast_shadow)
        {
            continue;
        }

        switch (obj->type)
        {
        case MAP_OBJ_STATIC_MESH:
        {
            Mesh* mesh = map->meshes[obj->static_mesh.mesh_id];

            renderer_draw_mesh_shadow(mesh, &obj->transform, &shd);
        }break;
            case MAP_OBJ_ANIMATED_MESH:
            {
                animation_t* anim = map->animations[obj->animated_mesh.mesh_id];

                //render mesh
                renderer_draw_animation_shadow(anim, &obj->transform, obj->animated_mesh.frame, &shd);
            }break;
        default:
            break;
        }
    }
}

Map* map_load()
{
    Map* map = (Map*)malloc(sizeof(Map));
    assert(map);

    map->num_materials = 2;
    map->materials = (Material**)malloc(sizeof(Material*) * map->num_materials);
    assert(map->materials);

    map->materials[0] = material_create();
    map->materials[0]->diffuse_map = texture_load("data/textures/stone_floor.tga", TEXTURE_FILTER_LINEAR);
    map->materials[0]->specular_map = texture_load("data/textures/wall.tga", TEXTURE_FILTER_LINEAR);

    map->materials[1] = material_create();
    map->materials[1]->diffuse_map = texture_load("data/textures/floor.tga", TEXTURE_FILTER_LINEAR);

    map->num_meshes = 3;
    map->meshes = (Mesh**)malloc(sizeof(Mesh*) * map->num_meshes);
    assert(map->meshes);

    MeshBuffer buffer;
    mesh_buffer_init(&buffer);
    //mesh_buffer_add_sphere(&buffer, 3, 16, 16, 0, color_white());
    if (!mesh_buffer_load_mesh(&buffer, "data/meshes/female.obj")) return NULL;

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

    map->num_animations = 1;
    map->animations = (animation_t**)malloc(sizeof(animation_t*) * map->num_animations);
    map->animations[0] = anim_load("data/meshes/test.anim");
    assert(map->animations[0]);

    map->num_collision_objects = 2;
    map->collision_objects = (CollisionObject*)malloc(sizeof(CollisionObject) * map->num_collision_objects);

    collision_object_create_sphere(&map->collision_objects[0], vec3_create(0,0,0),1);
    collision_object_create_sphere(&map->collision_objects[1], vec3_create(-3,0,0),1);

    map->num_objects = 4;
    map->objects = (MapObject*)malloc(sizeof(MapObject) * map->num_objects);
    assert(map->objects);

    MapObject* obj1 = &map->objects[0];
    obj1->type = MAP_OBJ_STATIC_MESH;
    obj1->static_mesh.mesh_id = 0;
    obj1->static_mesh.mat_id = 0;
    obj1->cast_shadow = true;
    transform_init(&obj1->transform, vec3_create(0,0,0), vec3_create(0,0,0), vec3_create(1,1,1));
    
    MapObject* obj2 = &map->objects[1];
    obj2->type = MAP_OBJ_STATIC_MESH;
    obj2->static_mesh.mesh_id = 1;
    obj2->static_mesh.mat_id = 1;
    obj2->cast_shadow = false;
    transform_init(&obj2->transform, vec3_create(0, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

    MapObject* obj3 = &map->objects[2];
    obj3->type = MAP_OBJ_STATIC_MESH;
    obj3->static_mesh.mesh_id = 2;
    obj3->static_mesh.mat_id = 0;
    obj3->cast_shadow = true;
    transform_init(&obj3->transform, vec3_create(10, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

    MapObject* obj4 = &map->objects[3];
    obj4->type = MAP_OBJ_ANIMATED_MESH;
    obj4->animated_mesh.mesh_id = 0;
    obj4->animated_mesh.mat_id = 0;
    obj4->animated_mesh.frame = 0;
    obj4->cast_shadow = true;
    transform_init(&obj4->transform, vec3_create(-2, 0, 0), vec3_create(0, 0, 0), vec3_create(1, 1, 1));

    map->sun_light.color = vec3_create(0.8f, 0.8f, 0.8f);
    map->sun_light.direction = vec3_create(0.0f, -1.0f, -0.5f);
    map->sun_light.intensity = 0.5f;

    map->num_point_lights = 3;
    map->point_lights = (PointLight*)malloc(sizeof(PointLight) * map->num_point_lights);
    assert(map->point_lights);

    map->point_lights[0].color = vec3_create(0, 0, 1);
    map->point_lights[0].position = vec3_create(0, 3, -3);
    map->point_lights[0].ambient_intensity = 0.5f;
    map->point_lights[0].diffuse_intensity = 1.0f;
    map->point_lights[0].atten.constant = 1;
    map->point_lights[0].atten.linear = 0.05f;
    map->point_lights[0].atten.exp = 0.05f;

    map->point_lights[1].color = vec3_create(0, 1, 0);
    map->point_lights[1].position = vec3_create(3, 3, 3);
    map->point_lights[1].ambient_intensity = 0.2f;
    map->point_lights[1].diffuse_intensity = 1.0f;
    map->point_lights[1].atten.constant = 1;
    map->point_lights[1].atten.linear = 0.05f;
    map->point_lights[1].atten.exp = 0.05f;

    map->point_lights[2].color = vec3_create(1, 0, 0);
    map->point_lights[2].position = vec3_create(-3, 3, 0);
    map->point_lights[2].ambient_intensity = 0.2f;
    map->point_lights[2].diffuse_intensity = 1.0f;
    map->point_lights[2].atten.constant = 1;
    map->point_lights[2].atten.linear = 0.05f;
    map->point_lights[2].atten.exp = 0.05f;

    Vec3 shadow_pos = vec3_create(0,0,0);
    Mat4 projection = mat4_perspective(45.0f, 512.0f / 512.0f, 0.1f, 100.0f);
    g_shadow_cam = cam_create(shadow_pos,projection);
    g_shadow_cam.max_distance = 40;
    g_shadow_cam.distance = 40;

    return map;
}
