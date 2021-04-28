#ifndef _TEK_LIGHT_HPP
#define _TEK_LIGHT_HPP

#include "tek_color.hpp"
#include "../math/tek_math.hpp"

typedef struct sun_light_s
{
    Vec3 direction;
    Vec3 color;
    float intensity;
}SunLight;

typedef struct
{
    float constant;
    float linear;
    float exp;
}Attenuation;

typedef struct point_light_s
{
    Vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    Vec3 position;
    Attenuation atten;
}PointLight;

Mat4 shadow_matrix(Vec3 position, Vec3 rotation, Mat4 projection);

typedef struct shadow_map_fbo_s
{
    u32 width;
    u32 height;
    u32 fbo;
    u32 shadow_map;
}ShadowMapFBO;

ShadowMapFBO* shadow_map_create(u32 window_width, u32 window_height);
void shadow_map_destroy(ShadowMapFBO* shadow_map);
void shadow_map_bind_for_writing(ShadowMapFBO* shadow_map);
void shadow_map_bind_for_reading(ShadowMapFBO* shadow_map, u32 texture_unit);
void shadow_map_unbind();

#endif
