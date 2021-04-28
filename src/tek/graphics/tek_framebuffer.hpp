#ifndef _TEK_FRAMEBUFFER_HPP
#define _TEK_FRAMEBUFFER_HPP

#include "../utils/tek_core.hpp"
#include "tek_rect.hpp"
#include "../math/tek_math.hpp"

typedef struct framebuffer_s
{
    u32 fbo;
    u32 tex_id;
    u32 depth_id;
    u32 width;
    u32 height;
}Framebuffer;

void fb_destroy(Framebuffer* fb);

void fb_bind_reading(Framebuffer* fb, u32 texture_unit);
void fb_bind_writing(Framebuffer* fb);

void fb_unbind();

Framebuffer* fb_create(u32 width, u32 height);


#endif
