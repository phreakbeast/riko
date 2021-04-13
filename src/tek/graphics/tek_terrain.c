#include "tek_terrain.h"

#include "tek_opengl.h"

TerrainVertexData terrain_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color)
{
    TerrainVertexData res;
    res.position = position;
    res.uv = uv;
    res.normal = normal;
    res.color = color;
    return res;
}

Terrain* terrain_create(TerrainVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
    Terrain* res = malloc(sizeof(Terrain));

    res->num_vertices = num_vertices;
    res->num_indices = num_indices;
    
    int size = num_vertices * sizeof(TerrainVertexData);
    
    GLCall(glGenVertexArrays(1, &res->vao));
    
    GLCall(glBindVertexArray(res->vao));
    
    GLCall(glGenBuffers(1, &res->vbo));
    GLCall(glGenBuffers(1, &res->ibo));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, res->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertexData),
                                 (const GLvoid*)(offsetof(TerrainVertexData, position))));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertexData),
                                 (const GLvoid*)(offsetof(TerrainVertexData, uv))));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertexData),
                                 (const GLvoid*)(offsetof(TerrainVertexData, normal))));
    GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TerrainVertexData),
                                 (const GLvoid*)(offsetof(TerrainVertexData, color))));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(u32), &indices[0], GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void terrain_destroy(Terrain* terrain)
{
    GLCall(glDeleteBuffers(1, &terrain->vbo));
    GLCall(glDeleteBuffers(1, &terrain->ibo));
    GLCall(glDeleteVertexArrays(1, &terrain->vao));
}

void terrain_render(Terrain* terrain)
{
    GLCall(glBindVertexArray(terrain->vao));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->ibo));
    GLCall(glDrawElements(GL_TRIANGLES, terrain->num_indices, GL_UNSIGNED_INT, 0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));
    GLCall(glBindVertexArray(0));
}
