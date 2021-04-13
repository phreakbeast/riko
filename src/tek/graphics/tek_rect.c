#include "tek_rect.h"

Rect rect_create(float x, float y, float w, float h)
{
    Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

Rect rect_uv(float x, float y, float w, float h, u32 width, u32 height)
{
    float l = x / (float) width;
    float r = l + w / (float) width;
    float t = (height - y) / (float) height;
    float b = t - h / (float) height;

    Rect rect;
    rect.x = l;
    rect.y = b;
    rect.w = r;
    rect.h = t;
    return rect;
}

const Rect rect_to_uv(Rect rect, u32 width, u32 height)
{
    float l = rect.x / (float) width;
    float r = l + rect.w / (float) width;
    float t = (height - rect.y) / (float) height;
    float b = t - rect.h / (float) height;

    Rect res = {l, b, r, t};
    return res;
}

const Vec4 rect_to_vec4(Rect rect)
{
    return vec4_create(rect.x, rect.y, rect.w, rect.h);
}

const Rect rect_move(Rect rect, float x, float y)
{
    Rect res;
    res.x = rect.x + x;
    res.y = rect.y + y;
    res.w = rect.w;
    res.h = rect.h;
    return res;
}

bool rect_is_inside(Rect rect, float x, float y)
{
    float min_x = rect.x;
    float max_x = rect.x + rect.w;
    float min_y = rect.y;
    float max_y = rect.y + rect.h;

    if (x > min_x && x <= max_x &&
        y > min_y && y <= max_y) 
    {
        return true;
    }

    return false;
}

const Rect rect_create_vertical_slice(Rect rect, float start, float end)
{
    float res_y = rect.y + rect.h * start;
    float res_h = rect.h * end - rect.h * start;
    Rect res = {rect.x, res_y, rect.w, res_h};
    return res;
}

const Rect rect_create_horizontal_slice(Rect rect, float start, float end)
{
    float res_x = rect.x + rect.w * start;
    float res_w = rect.w * end - rect.w * start;
    Rect res = {res_x, rect.y, res_w, rect.h};
    return res;
}

