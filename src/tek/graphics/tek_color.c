#include "tek_color.h"

#include "../math/tek_math.h"

static Color g_white = { 255,255,255,255 };
static Color g_black = { 0,0,0,255 };
static Color g_red = { 255,0,0,255 };
static Color g_green = { 0,255,0,255 };
static Color g_blue = { 0,0,255,255 };
static Color g_yellow = { 255,255,0,255 };

Color color_create(u8 r, u8 g, u8 b)
{
    Color res;
    res.r = r;
    res.g = g;
    res.b = b;
    res.a = 255;
    return res;
}

Color color_create_alpha(u8 r, u8 g, u8 b, u8 a)
{
    Color res;
    res.r = r;
    res.g = g;
    res.b = b;
    res.a = a;
    return res;
}

const u32 color_to_int(Color color)
{
    return (color.a << 24 | color.b << 16 | color.g << 8 | color.r);
}

const Vec3 color_to_vec3(Color color)
{
    
    return vec3_create(
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f);
}

const Vec4 color_to_vec4(Color color)
{
    
    return vec4_create(
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f,
            color.a / 255.0f);
}

const Color color_lighten(Color color, u8 shade)
{
    //TODO: prevent overflow
    Color res;
    res.r = color.r + shade;
    res.g = color.g + shade;
    res.b = color.b + shade;
    res.a = color.a;
    return res;
}

const Color color_darken(Color color, u8 shade)
{
    //TODO: prevent underflow
    Color res;
    res.r = color.r - shade;
    res.g = color.g - shade;
    res.b = color.b - shade;
    res.a = color.a;
    return res;
}

const Color color_lerp(Color c1, Color c2, float t)
{
    float rt = (float)c1.r + t * (c2.r - c1.r);
    float gt = (float)c1.g + t * (c2.g - c1.g);
    float bt = (float)c1.b + t * (c2.b - c1.b);
    float at = (float)c1.a + t * (c2.a - c1.a);
    
    return color_create_alpha((u8)rt, (u8)gt, (u8)bt, (u8)at);
}

const u32 color_floats_to_int(float r, float g, float b, float a)
{
    u8 ri = (u8)(r * 255);
    u8 gi = (u8)(g * 255);
    u8 bi = (u8)(b * 255);
    u8 ai = (u8)(a * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 color_vec4_to_int(Vec4 color)
{
    u8 ri = (u8)(color.x * 255);
    u8 gi = (u8)(color.y * 255);
    u8 bi = (u8)(color.z * 255);
    u8 ai = (u8)(color.w * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 color_vec3_to_int(Vec3 color)
{
    u8 ri = (u8)(color.x * 255);
    u8 gi = (u8)(color.y * 255);
    u8 bi = (u8)(color.z * 255);
    u8 ai = 255;
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const Color color_white()
{
    return g_white;
}

const Color color_black()
{
    return g_black;
}

const Color color_red()
{
    return g_red;
}

const Color color_green()
{
    return g_green;
}

const Color color_blue()
{
    return g_blue;
}

const Color color_yellow()
{
    return g_yellow;
}

const Color color_random()
{
    u8 r = (u8)math_get_random_int(0, 255);
    u8 g = (u8)math_get_random_int(0, 255);
    u8 b = (u8)math_get_random_int(0, 255);
    return color_create(r, g, b);
}
