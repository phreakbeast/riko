#ifndef _TEK_GBUFFER_H
#define _TEK_GBUFFER_H

#include "../utils/tek_core.h"

typedef enum gbuffer_texture_type_e
{
    GBUFFER_TEXTURE_TYPE_POSITION,
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_TEXTURE_TYPE_TEXCOORD,
    GBUFFER_NUM_TEXTURES
}GBufferTextureType;

typedef struct gbuffer_s
{
    u32 fbo;
    u32 textures[GBUFFER_NUM_TEXTURES];
    u32 depth_texture;
}GBuffer;

GBuffer* gbuffer_create(u32 width, u32 height);
void gbuffer_destroy(GBuffer* buffer);

void gbuffer_bind_for_writing(GBuffer* buffer);
void gbuffer_bind_for_reading(GBuffer* buffer);
void gbuffer_set_read_buffer(GBuffer* buffer, GBufferTextureType texture_type);

#endif