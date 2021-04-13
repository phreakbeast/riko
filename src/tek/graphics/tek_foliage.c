#include "tek_foliage.h"

#include "tek_opengl.h"
#include "tek_shader.h"


Foliage* foliage_create(Vec3* positions, u32 num_positions, Rect rect, Rect uv)
{
    Foliage* res = malloc(sizeof(Foliage));
    res->num_positions = num_positions;
    
    GLCall(glGenVertexArrays(1, &res->vao));
    GLCall(glBindVertexArray(res->vao));
    
    glGenBuffers(1, &res->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, res->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * num_positions, &positions[0], GL_STATIC_DRAW);

    return res;
}

void foliage_destroy(Foliage* foliage)
{
    GLCall(glDeleteBuffers(1, &foliage->vbo));
    GLCall(glDeleteVertexArrays(1, &foliage->vao));
}

void foliage_render(Foliage* foliage)
{
    GLCall(glBindVertexArray(foliage->vao));
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, foliage->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0); // position
    
    glDrawArrays(GL_POINTS, 0, foliage->num_positions);
    
    glDisableVertexAttribArray(0);
    GLCall(glBindVertexArray(0));
}
