#ifndef _TEK_MATERIAL_H
#define _TEK_MATERIAL_H

#include "tek_texture.h"
#include "tek_color.h"

typedef struct material_s
{
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;

    Texture* diffuse_map;
    Texture* specular_map;
    Texture* normal_map;

    float specular_level;
    float specular_power;
}Material;

Material* material_create();
void material_destroy(Material* mat);

#endif