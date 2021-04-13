#include "tek_picking.h"

#include <stdio.h>

PickVertexData pick_vertex_data_create(Vec3 position, Vec3 color)
{
    PickVertexData res;
    res.position = position;
    res.color = color;
    return res;
}

void pick_mesh_destroy(PickMesh* mesh)
{
    GLCall(glDeleteBuffers(5, mesh->vbo));
    GLCall(glDeleteVertexArrays(1, &mesh->vao));
}

PickMesh* pick_mesh_create(PickVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
    PickMesh* mesh = malloc(sizeof(PickMesh));
    
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;
    
    int size = num_vertices * sizeof(PickVertexData);
    int stride = sizeof(PickVertexData);
    
    GLCall(glGenVertexArrays(1, &mesh->vao));
    GLCall(glBindVertexArray(mesh->vao));
    
    GLCall(glGenBuffers(5, mesh->vbo));
    
    // Generate and populate the buffers with vertex attributes and the indices
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(PickVertexData, position))));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(PickVertexData, color))));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * num_indices, indices, GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]));
    
    for(unsigned int i = 0; i < 4; i++)
    {
        GLCall(glEnableVertexAttribArray(4 + i));
        GLCall(glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),(const GLvoid*)(sizeof(GLfloat) * i * 4)));
        GLCall(glVertexAttribDivisor(4 + i, 1));
    }
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[3]));
    
    for(unsigned int i = 0; i < 4; i++)
    {
        GLCall(glEnableVertexAttribArray(8 + i));
        GLCall(glVertexAttribPointer(8 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4)));
        GLCall(glVertexAttribDivisor(8 + i, 1));
    }
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[4]));

    GLCall(glEnableVertexAttribArray(13));
    GLCall(glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), NULL));
    //GLCall(glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3),(const GLvoid*)(sizeof(GLfloat) * 3)));
    GLCall(glVertexAttribDivisor(13, 1));
    
    return mesh;
}

void pick_mesh_render(PickMesh* mesh)
{
    GLCall(glBindVertexArray(mesh->vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]));
    //GLCall(glDrawArrays(GL_TRIANGLES, 0, num_vertices));
    GLCall(glDrawElements(GL_TRIANGLES, mesh->num_indices,GL_UNSIGNED_INT,0));
    //glDrawElementsInstancedBaseVertex(GL_TRIANGLES,num_indices,GL_UNSIGNED_INT,0,num_instances,0);
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void pick_mesh_render_instanced(PickMesh* mesh, Mat4* mvp_mats, Mat4* world_mats, Vec3* colors, u32 num_instances)
{
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4) * num_instances, mvp_mats, GL_DYNAMIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[3]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4) * num_instances, world_mats, GL_DYNAMIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[4]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * num_instances, colors, GL_DYNAMIC_DRAW));
    
    GLCall(glBindVertexArray(mesh->vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]));
    //GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices));
    GLCall(glDrawElementsInstancedBaseVertex(GL_TRIANGLES, mesh->num_indices,GL_UNSIGNED_INT, NULL, num_instances, 0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void pick_texture_destroy(PickingTexture* texture)
{
    GLCall(glDeleteTextures(1, &texture->picking_texture));
    GLCall(glDeleteTextures(1, &texture->depth_texture));
    GLCall(glDeleteFramebuffers(1, &texture->fbo));
}

PickingTexture* pick_texture_create(int width, int height)
{
    PickingTexture* res = malloc(sizeof(PickingTexture));
    
    res->width = width;
    res->height = height;
    
    // Create the FBO
    GLCall(glGenFramebuffers(1, &res->fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, res->fbo));
    
    // Create the texture object for the primitive information buffer
    GLCall(glGenTextures(1, &res->picking_texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, res->picking_texture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res->picking_texture, 0));
    
    // Create the texture object for the depth buffer
    GLCall(glGenTextures(1, &res->depth_texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, res->depth_texture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, res->depth_texture, 0));
    
    // Disable reading to avoid problems with older GPUs
    GLCall(glReadBuffer(GL_NONE));
    //glReadBuffer(GL_COLOR_ATTACHMENT0);
    
    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
    
    // Verify that the FBO is correct
    //GLCall(GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    
    /*
    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("FB error, status: 0x%x\n", Status);
        return nullptr;
    }
     */
    
    // Restore the default framebuffer
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    
    return res;
}

void pick_texture_enable_drawing(PickingTexture* texture)
{
    GLCall(glDisable(GL_BLEND));
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texture->fbo));
}

void pick_texture_disable_drawing(PickingTexture* texture)
{
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    GLCall(glEnable(GL_BLEND));
}

void pick_texture_bind_for_reading(PickingTexture* texture, GLenum texture_unit)
{
    GLCall(glActiveTexture(texture_unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->picking_texture));
}

PixelInfo pick_texture_get_pixel(PickingTexture* texture, u32 x, u32 y)
{
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, texture->fbo));
    GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
    
    PixelInfo pixel;
    GLCall(glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel));
    
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    
    return pixel;
}
