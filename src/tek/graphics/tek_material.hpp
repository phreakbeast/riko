#ifndef _TEK_MATERIAL_HPP
#define _TEK_MATERIAL_HPP

#include "tek_texture.hpp"
#include "tek_color.hpp"

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