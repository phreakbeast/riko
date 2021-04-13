#include "tek_spritebatch.h"

#include "tek_opengl.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define SB_MAX_TEXTURES 16

typedef struct 
{
    Vec3 position;
    Vec2 uv;
    float tex_id;
    u32 color;
} SbVertexData;

static u32 g_num_sprites;
static u32 g_drawcalls;

static u64 g_max_sprites;
static SbVertexData* g_buffer;

static u32 g_texture_slots[SB_MAX_TEXTURES];
static u32 g_num_slots;
static Shader* g_shader;
static Mat4 g_ortho;
static u32 g_vao;
static u32 g_vbo;
static u32 g_ibo;
static u32 g_index_count;

void sb_destroy()
{
    GLCall(glDeleteBuffers(1, &g_vbo));
    GLCall(glDeleteBuffers(1, &g_ibo));
    GLCall(glDeleteVertexArrays(1, &g_vao));
    shader_destroy(g_shader);
    free(g_shader);
}

bool sb_init(u32 width, u32 height, u64 max_sprites)
{
    g_num_sprites = 0;
    g_drawcalls = 0;
    
    g_buffer = NULL;
    
    g_num_slots = 0;
    for(u32 i = 0; i < SB_MAX_TEXTURES; ++i)
    {
        g_texture_slots[i] = 0;
    }
    
    g_vao = 0;
    g_vbo = 0;
    g_ibo = 0;
    g_index_count = 0;
    
    g_max_sprites = max_sprites;
    u64 vertex_size = sizeof(SbVertexData);
    u64 sprite_size = vertex_size * 4;
    u64 buffer_size = sprite_size * max_sprites;
    u64 indices_size = max_sprites * 6;
    
    g_ortho = mat4_ortho(0, (float)width, 0, (float)height, 1, -1);
    g_ortho = mat4_transposed(&g_ortho);
    
    //Shader
    g_shader = shader_load("sprite.glsl");
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
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size,
                                 (const GLvoid*)(offsetof(SbVertexData, position))));
    
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size,
                                 (const GLvoid*)(offsetof(SbVertexData, uv))));
    
    GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, vertex_size,
                                 (const GLvoid*)(offsetof(SbVertexData, tex_id))));
    
    GLCall(glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertex_size,
                                 (const GLvoid*)(offsetof(SbVertexData, color))));
    
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    
    GLuint* indices = malloc(sizeof(GLuint) * indices_size);
    assert(indices);

    int offset = 0;
    for(u64 i = 0; i < indices_size; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        
        indices[i + 3] = offset + 0;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 3;
        
        offset += 4;
    }
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(GLuint), indices, GL_STATIC_DRAW));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    free(indices);
    
    return true;
}

void sb_begin()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, g_vbo));
    GLCall(g_buffer = (SbVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
}

void sb_end()
{
    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}

void sb_flush()
{
    shader_bind(g_shader);
    
    shader_uniform_mat4(g_shader, 0, &g_ortho, 1, false);
    
    shader_uniform_int(g_shader, 1, 0);
    shader_uniform_int(g_shader, 2, 1);
    shader_uniform_int(g_shader, 3, 2);
    shader_uniform_int(g_shader, 4, 3);
    shader_uniform_int(g_shader, 5, 4);
    shader_uniform_int(g_shader, 6, 5);
    shader_uniform_int(g_shader, 7, 6);
    shader_uniform_int(g_shader, 8, 7);
    
    for(u32 i = 0; i < g_num_slots; ++i)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));
        GLCall(glBindTexture(GL_TEXTURE_2D, g_texture_slots[i]));
    }
    
    GLCall(glBindVertexArray(g_vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo));
    GLCall(glDrawElements(GL_TRIANGLES, g_index_count, GL_UNSIGNED_INT, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));
    GLCall(glBindVertexArray(0));
    
    //TODO: clear texture slots
    g_drawcalls++;
    g_index_count = 0;
    g_num_slots = 0;
    g_num_sprites = 0;
}

void sb_resize(u32 width, u32 height)
{
    g_ortho = mat4_ortho(0, (float)width, 0, (float)height, 1, -1);
    g_ortho = mat4_transposed(&g_ortho);
}

void sb_reset_stats()
{
    g_drawcalls = 0;
    g_num_sprites = 0;
}

void sb_render_rect(Rect rect, Color color)
{
    
    if(g_num_sprites >= g_max_sprites)
    {
        sb_end();
        sb_flush();
        sb_begin();
    }
    
    const Vec3 position = {rect.x, rect.y, 0};
    const Vec2 size = {rect.w, rect.h};
    const u32 col = color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;
    
    {
        g_buffer->position = position;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x, position.y + size.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x + size.x, position.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    g_num_sprites++;
    g_index_count += 6;
}

void sb_render_sprite(Rect dest, Rect src, u32 texture_id)
{
    
    if(g_num_sprites >= g_max_sprites)
    {
        sb_end();
        sb_flush();
        sb_begin();
    }
    
    const Vec3 position = {dest.x, dest.y, 0};
    const Vec2 size = {dest.w, dest.h};
    const u32 col = color_floats_to_int(1, 1, 1, 1);
    const GLuint tid = texture_id;
    
    Vec2 uv[4];
    uv[0].x = src.x;
    uv[0].y = src.h;
    uv[1].x = src.x;
    uv[1].y = src.y;
    uv[2].x = src.w;
    uv[2].y = src.y;
    uv[3].x = src.w;
    uv[3].y = src.h;
    
    float ts = 0.0f;
    if(tid > 0)
    {
        u32 found = 0;
        for(u32 i = 0; i < g_num_slots; i++)
        {
            if(g_texture_slots[i] == tid)
            {
                ts = (float)(i + 1);
                found = 1;
                break;
            }
        }
        
        if(!found)
        {
            if(g_num_slots >= SB_MAX_TEXTURES)
            {
                sb_begin();
                sb_end();
                sb_flush();
            }
            else
            {
                g_texture_slots[g_num_slots++] = tid;
                ts = (float)(g_num_slots);
            }
        }
    }
    
    
    {
        g_buffer->position = position;
        g_buffer->uv = uv[0];
        g_buffer->tex_id = ts;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x, position.y + size.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv[1];
        g_buffer->tex_id = ts;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv[2];
        g_buffer->tex_id = ts;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        Vec3 new_pos = {position.x + size.x, position.y, position.z};
        g_buffer->position = new_pos;
        g_buffer->uv = uv[3];
        g_buffer->tex_id = ts;
        g_buffer->color = col;
        g_buffer++;
    }
    
    g_num_sprites++;
    g_index_count += 6;
}

void sb_render_polygon(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color color)
{
    
    if(g_num_sprites >= g_max_sprites)
    {
        sb_end();
        sb_flush();
        sb_begin();
    }
    
    const Vec3 v0 = {p0.x, p0.y, 0};
    const Vec3 v1 = {p1.x, p1.y, 0};
    const Vec3 v2 = {p2.x, p2.y, 0};
    const Vec3 v3 = {p3.x, p3.y, 0};
    
    const u32 col = color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;
    
    {
        g_buffer->position = v0;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        g_buffer->position = v1;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        g_buffer->position = v2;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    {
        g_buffer->position = v3;
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
    }
    
    g_num_sprites++;
    g_index_count += 6;
}

void sb_render_text(const char* text, BitmapFont* font, int x, int y, Color color, float max_width)
{
    if(g_num_sprites >= g_max_sprites)
    {
        sb_end();
        sb_flush();
        sb_begin();
    }
    
    size_t len = strlen(text);
    int pos_x = x;
    int pos_y = y;
    
    if(max_width != 0)
    {
        //check if the text is too long
        float width = (float)(len * font->width);
        if(width > max_width)
        {
            float diff = max_width / font->width;
            len = (size_t)diff;
        }
    }
    
    for(u32 i = 0; i < len; ++i)
    {
        char c = text[i];
        
        if(c == '\n')
        {
            pos_y += font->height;
            pos_x = x;
            continue;
        }
        
        const FontLetter* letter = bitmapfont_get_letter(font,c);
        if(letter == NULL)
        {
            letter = bitmapfont_get_letter(font, '?');
        }
        
        const Vec3 position = {(float)pos_x, (float)pos_y, 0};
        const Vec2 size = {(float)letter->width, (float)letter->width};
        const unsigned int col = color_to_int(color);
        Vec2 uv[4];
        
        uv[0].x = letter->uv_l;
        uv[0].y = letter->uv_t;
        uv[1].x = letter->uv_l;
        uv[1].y = letter->uv_b;
        uv[2].x = letter->uv_r;
        uv[2].y = letter->uv_b;
        uv[3].x = letter->uv_r;
        uv[3].y = letter->uv_t;
        
        const GLuint tid = font->texture->id;
        
        float ts = 0.0f;
        if(tid > 0)
        {
            u32 found = 0;
            for(u32 i = 0; i < g_num_slots; i++)
            {
                if(g_texture_slots[i] == tid)
                {
                    ts = (float)(i + 1);
                    found = 1;
                    break;
                }
            }
            
            if(!found)
            {
                if(g_num_slots >= SB_MAX_TEXTURES)
                {
                    sb_begin();
                    sb_end();
                    sb_flush();
                }
                
                g_texture_slots[g_num_slots++] = tid;
                ts = (float)(g_num_slots);
            }
        }
        
        {
            g_buffer->position = position;
            g_buffer->uv = uv[0];
            g_buffer->tex_id = ts;
            g_buffer->color = col;
            g_buffer++;
        }
        
        {
            Vec3 new_pos = {position.x, position.y + size.y, position.z};
            g_buffer->position = new_pos;
            g_buffer->uv = uv[1];
            g_buffer->tex_id = ts;
            g_buffer->color = col;
            g_buffer++;
        }
        
        {
            Vec3 new_pos = {position.x + size.x, position.y + size.y, position.z};
            g_buffer->position = new_pos;
            g_buffer->uv = uv[2];
            g_buffer->tex_id = ts;
            g_buffer->color = col;
            g_buffer++;
        }
        
        {
            Vec3 new_pos = {position.x + size.x, position.y, position.z};
            g_buffer->position = new_pos;
            g_buffer->uv = uv[3];
            g_buffer->tex_id = ts;
            g_buffer->color = col;
            g_buffer++;
        }
        
        g_index_count += 6;
        pos_x += letter->width;
        g_num_sprites++;
    }
}

void sb_render_circle(Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments, Color color)
{
    //TODO: check if buffers is big enough
    
    Vec3* points = malloc(sizeof(Vec3) * (num_segments + 2));
    
    float step = 0;
    if(end_angle > start_angle)
    {
        step = (end_angle - start_angle) / num_segments;
    }
    else
    {
        step = ((360 - start_angle) + end_angle) / num_segments;
    }
    
    for(u32 i = 0; i < num_segments + 1; ++i)
    {
        float angle = start_angle + i * step;
        float x, y;
        math_get_circle_coord(radius, pos.x, pos.y, angle, &x, &y);
        points[i].x = x;
        points[i].y = y;
        points[i].z = 0;
    }
    
    points[num_segments + 1].x = pos.x;
    points[num_segments + 1].y = pos.y;
    points[num_segments + 1].z = 0;
    
    const u32 col = color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;
    
    for(u32 i = 0; i < num_segments + 1; ++i)
    {
        g_buffer->position = points[i];
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
        
        g_buffer->position = points[num_segments + 1];
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
        
        g_buffer->position = points[num_segments + 1];
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
        
        g_buffer->position = points[i + 1];
        g_buffer->uv = uv;
        g_buffer->tex_id = tid;
        g_buffer->color = col;
        g_buffer++;
        
        g_index_count += 6;
        g_num_sprites++;
    }
    
    free(points);
}

void sb_render_line(Vec2 p0, Vec2 p1, float width, Color color)
{
    
    if(g_num_sprites >= g_max_sprites)
    {
        sb_end();
        sb_flush();
        sb_begin();
    }
        
    Vec3 new_pos[4];
    
    Vec2 vec_l = p0;
    Vec2 vec_r = p1;
    if(p0.x > p1.x)
    {
        vec_l = p1;
        vec_r = p0;
    }
    
    Vec2 vec = vec2_sub(vec_r, vec_l);
    Vec2 vec_p = vec2_create(-vec.y, vec.x);    
    
    Vec2 vec_p2 = vec2_create(vec.y, -vec.x);            
    
    Vec2 vec_n1 = vec2_normalized(vec_p);
    Vec2 vec_n2 = vec2_normalized(vec_p2);
    
    Vec2 v0 = vec2_add(vec_l, (vec2_mul(vec_n2, width)));
    Vec2 v1 = vec2_add(vec_l, (vec2_mul(vec_n1, width)));
    Vec2 v2 = vec2_add(vec_r, (vec2_mul(vec_n1, width)));
    Vec2 v3 = vec2_add(vec_r, (vec2_mul(vec_n2, width)));
    
    new_pos[0].x = v0.x;
    new_pos[0].y = v0.y;
    new_pos[0].z = 0;
    
    new_pos[1].x = v1.x;
    new_pos[1].y = v1.y;
    new_pos[1].z = 0;
    
    new_pos[2].x = v2.x;
    new_pos[2].y = v2.y;
    new_pos[2].z = 0;
    
    new_pos[3].x = v3.x;
    new_pos[3].y = v3.y;
    new_pos[3].z = 0;
    
    const u32 col = color_to_int(color);
    const Vec2 uv = {0, 0};
    const float tid = 0;
    
    g_buffer->position = new_pos[0];
    g_buffer->uv = uv;
    g_buffer->tex_id = tid;
    g_buffer->color = col;
    g_buffer++;
    
    g_buffer->position = new_pos[1];
    g_buffer->uv = uv;
    g_buffer->tex_id = tid;
    g_buffer->color = col;
    g_buffer++;
    
    g_buffer->position = new_pos[2];
    g_buffer->uv = uv;
    g_buffer->tex_id = tid;
    g_buffer->color = col;
    g_buffer++;
    
    g_buffer->position = new_pos[3];
    g_buffer->uv = uv;
    g_buffer->tex_id = tid;
    g_buffer->color = col;
    g_buffer++;
    
    g_index_count += 6;
}
