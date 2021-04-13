#ifndef _TEK_FRAMEBUFFER_H
#define _TEK_FRAMEBUFFER_H

#include "../utils/tek_core.h"
#include "tek_rect.h"
#include "../math/tek_math.h"

typedef struct framebuffer_s
{
    u32 fbo;
    u32 tex_id;
    u32 depth_id;

    u32 width;
    u32 height;    
}Framebuffer;

Framebuffer* fb_create(u32 width, u32 height);
void fb_destroy(Framebuffer* fb);

void fb_bind_reading(Framebuffer* fb, u32 texture_unit);
void fb_bind_writing(Framebuffer* fb);

void fb_unbind();

#endif
