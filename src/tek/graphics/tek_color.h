#ifndef _TEK_COLOR_H
#define _TEK_COLOR_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"

typedef struct color_s
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;        
}Color;

Color color_create(u8 r, u8 g, u8 b);
Color color_create_alpha(u8 r, u8 g, u8 b, u8 a);

const u32 color_to_int(Color color);

const Vec3 color_to_vec3(Color color);

const Vec4 color_to_vec4(Color color);

const Color color_lighten(Color color, u8 shade);

const Color color_darken(Color color, u8 shade);

const Color color_lerp(Color c1, Color c2, float t);

const u32 color_floats_to_int(float r, float g, float b, float a);
const u32 color_vec4_to_int(Vec4 color);
const u32 color_vec3_to_int(Vec3 color);

const Color color_white();
const Color color_black();
const Color color_red();
const Color color_green();
const Color color_blue();
const Color color_yellow();
const Color color_random();

#endif
