#include "tek_gbuffer.h"

#include "tek_opengl.h"
#include <stdio.h>

GBuffer* gbuffer_create(u32 width, u32 height)
{
    GBuffer* buffer = malloc(sizeof(GBuffer));
    // Create the FBO
    glGenFramebuffers(1, &buffer->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer->fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(buffer->textures), buffer->textures);
    glGenTextures(1, &buffer->depth_texture);

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(buffer->textures); i++) 
    {
        glBindTexture(GL_TEXTURE_2D, buffer->textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, buffer->textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, buffer->depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, buffer->depth_texture, 0);

    GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) 
    {
        printf("FB error, status: 0x%x\n", status);
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void gbuffer_destroy(GBuffer* buffer)
{
    glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(buffer->textures), buffer->textures);
    glDeleteTextures(1, &buffer->depth_texture);
    glDeleteFramebuffers(1, &buffer->fbo);
}

void gbuffer_bind_for_writing(GBuffer* buffer)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer->fbo);
}

void gbuffer_bind_for_reading(GBuffer* buffer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer->fbo);
}

void gbuffer_set_read_buffer(GBuffer* buffer, GBufferTextureType texture_type)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + texture_type);
}
