#ifndef _TEK_TEXTURE_HPP
#define _TEK_TEXTURE_HPP

#include "../utils/tek_core.hpp"
#include "tek_color.hpp"
#include "tek_image.hpp"

typedef enum texture_filter_e
{
    TEXTURE_FILTER_NEAREST,
    TEXTURE_FILTER_LINEAR
}TextureFilter;

typedef struct texture_s
{
    u32 id;
    u32 width;
    u32 height;    
}Texture;

void texture_destroy(Texture* texture);

void texture_bind(Texture* texture, u32 texture_unit);

Texture* texture_load(const char* filename, TextureFilter filter);
Texture* texture_create(Image* image, TextureFilter filter);


#endif
