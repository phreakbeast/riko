#include "tek_shapebuffer.h"

#include "tek_shape.h"
#include "tek_shader.h"
#include <stdlib.h>

static u32 g_num_lines;
static u32 g_drawcalls;

static u64 g_max_shapes;
static ShapeVertexData* g_buffer;

static Shader* g_shader;

static Camera* g_cam;

static u32 g_vao;
static u32 g_vbo;
static u32 g_ibo;
static u32 g_shape_index_count;

bool sbuffer_init(u64 max_shapes)
{
    g_num_lines = 0;
    g_drawcalls = 0;
    
    g_buffer = NULL;
    
    g_vao = 0;
    g_vbo = 0;
    g_ibo = 0;
    g_shape_index_count = 0;
    
    g_max_shapes = max_shapes;
    u64 vertex_size = sizeof(ShapeVertexData);
    u64 sprite_size = vertex_size * 2;
    u64 buffer_size = sprite_size * max_shapes;
    u64 indices_size = max_shapes * 2;
    
    //Shader
    g_shader = shader_load("shape.glsl");
    if(!g_shader)
    {
        return false;
    }
    
    GLCall(glGenVertexArrays(1, &g_vao));
    GLCall(glBindVertexArray(g_vao));
    
    GLCall(glGenBuffers(1, &g_vbo));
    GLCall(glGenBuffers(1, &g_ibo));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, g_vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size,
                                 (const GLvoid*)(offsetof(ShapeVertexData, position))));
    
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size,
                                 (const GLvoid*)(offsetof(ShapeVertexData, color))));
    
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    
    GLuint* indices = malloc(sizeof(GLuint) * indices_size);
    
    for(int i = 0; i < indices_size; i += 2)
    {
        indices[i + 0] = i + 0;
        indices[i + 1] = i + 1;
    }
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(GLuint), indices, GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    free(indices);
    
    return true;
}

void sbuffer_destroy()
{
    GLCall(glDeleteBuffers(1, &g_vbo));
    GLCall(glDeleteBuffers(1, &g_ibo));
    GLCall(glDeleteVertexArrays(1, &g_vao));
    shader_destroy(g_shader);
    free(g_shader);
}

void sbuffer_set_cam(Camera* cam)
{
    g_cam = cam;
}

void sbuffer_begin()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, g_vbo));
    GLCall(g_buffer = (ShapeVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
}

void sbuffer_end()
{
    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}

void sbuffer_flush()
{
    shader_bind(g_shader);
    
    Mat4 mvp = mat4_mul2(&g_cam->projection, &g_cam->view);
    mvp = mat4_transposed(&mvp);
    
    shader_uniform_mat4(g_shader, 0, &mvp, 1, false);
    
    GLCall(glBindVertexArray(g_vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo));
    GLCall(glDrawElements(GL_LINES, g_shape_index_count, GL_UNSIGNED_INT, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    
    //TODO: clear texture slots
    g_drawcalls++;
    g_shape_index_count = 0;
}

void sbuffer_render_line(Vec3 p0, Vec3 p1, Color color)
{
    
    if(g_num_lines >= g_max_shapes)
    {
        sbuffer_end();
        sbuffer_flush();
        sbuffer_begin();
    }
    
    const Vec3 col = color_to_vec3(color);
    
    {
        g_buffer->position = p0;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        g_buffer->position = p1;
        g_buffer->color = col;
        g_buffer++;
    }
    
    g_shape_index_count += 2;
    g_num_lines++;
}
