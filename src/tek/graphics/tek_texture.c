#include "tek_texture.h"

#include "tek_opengl.h"

#include <stdio.h>
#include <stdlib.h>

void texture_destroy(Texture* texture)
{
    GLCall(glDeleteTextures(1, &texture->id));
}

void texture_bind(Texture* texture, u32 texture_unit)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
}

Texture* texture_load(const char* filename, TextureFilter filter)
{
    Image* img = image_load(filename);
    if(!img)
    {
        return NULL;
    }
    
    Texture* res = texture_create(img, filter);
    
    image_destroy(img);
    free(img);
    
    return res;
}

Texture* texture_create(Image* image, TextureFilter filter)
{
    Texture* tex = malloc(sizeof(Texture));
    tex->width = image->width;
    tex->height = image->height;
    
    GLenum filtering = GL_NEAREST;
    switch(filter)
    {
        case TEXTURE_FILTER_NEAREST:filtering = GL_NEAREST;
            break;
        case TEXTURE_FILTER_LINEAR:filtering = GL_LINEAR;
            break;
        default:break;
    }
    
    GLenum tex_format = GL_RGB;
    if(image->format == IMAGE_FORMAT_RGBA)
    {
        tex_format = GL_RGBA;
    }
    
    GLCall(glGenTextures(1, &tex->id));
    
    GLCall(glBindTexture(GL_TEXTURE_2D, tex->id));
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering));
    
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, tex_format, image->width, image->height, 0, tex_format,
                        GL_UNSIGNED_BYTE, (GLvoid*)(image->pixels)));
    
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    
    return tex;
}

