#include "tek_water.h"

#include "tek_opengl.h"
#include <stddef.h>

WaterVertexData water_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal)
{
    WaterVertexData res;
    res.position = position;
    res.uv = uv;
    res.normal = normal;
    return res;
}

Water* water_create(u32 width, u32 height, float y)
{
    Water* res = malloc(sizeof(Water));

    res->num_vertices = 4;
    res->num_indices = 6;
 
    WaterVertexData vertices[4];
    u32 indices[6];
    
    float w = width;
    float d = height;
    
    vertices[0] = water_vertex_data_create(vec3_create(0,y,0),vec2_create(0,0),vec3_create(0,1,0));
    vertices[1] = water_vertex_data_create(vec3_create(0,y,d),vec2_create(0,0),vec3_create(0,1,0));
    vertices[2] = water_vertex_data_create(vec3_create(w,y,d),vec2_create(0,0),vec3_create(0,1,0));
    vertices[3] = water_vertex_data_create(vec3_create(w,y,0),vec2_create(0,0),vec3_create(0,1,0));
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    
    int size = res->num_vertices * sizeof(WaterVertexData);
    
    GLCall(glGenVertexArrays(1, &res->vao));
    GLCall(glBindVertexArray(res->vao));
    
    GLCall(glGenBuffers(1, &res->vbo));
    GLCall(glGenBuffers(1, &res->ibo));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, res->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    
    int stride = sizeof(WaterVertexData);
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof(WaterVertexData, position))));
    GLCall(glEnableVertexAttribArray(0));
    
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof(WaterVertexData, uv))));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof(WaterVertexData, normal))));
    GLCall(glEnableVertexAttribArray(2));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, res->num_indices * sizeof(u32), indices, GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void water_destroy(Water* water)
{
    GLCall(glDeleteBuffers(1, &water->vbo));
    GLCall(glDeleteBuffers(1, &water->ibo));
    GLCall(glDeleteVertexArrays(1, &water->vao));
}

void water_render(Water* water)
{
    GLCall(glBindVertexArray(water->vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, water->ibo));
    GLCall(glDrawElements(GL_TRIANGLES, water->num_indices, GL_UNSIGNED_INT, 0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}