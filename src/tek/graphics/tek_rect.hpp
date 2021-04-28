#ifndef _TEK_RECT_HPP
#define _TEK_RECT_HPP

#include "../utils/tek_core.hpp"
#include "../math/tek_math.hpp"

typedef struct rect_s
{
    float x;
    float y;
    float w;
    float h;    
}Rect;

Rect rect_create(float x, float y, float w, float h);
Rect rect_uv(float x, float y, float w, float h, u32 width, u32 height);

const Rect rect_to_uv(Rect rect, u32 width, u32 height);
const Vec4 rect_to_vec4(Rect rect);
const Rect rect_move(Rect rect, float x, float y);
bool rect_is_inside(Rect rect, float x, float y);
const Rect rect_create_vertical_slice(Rect rect, float start, float end);
const Rect rect_create_horizontal_slice(Rect rect, float start, float end);

#endif
