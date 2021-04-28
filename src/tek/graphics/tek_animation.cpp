#include "tek_animation.hpp"

#include "../math/tek_math.hpp"
#include "tek_opengl.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
    Vec3 next_position;
}anim_vertex_data_t;

typedef struct
{
    anim_vertex_data_t* vertices;
}internal_frame_t;

animation_t* anim_load(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }

    animation_t* res = (animation_t*)malloc(sizeof(animation_t));

    res->frames = NULL;
    res->num_frames = 0;
    res->vao = 0;
    res->ibo = 0;

    int cur_frame = 0;
    int cur_vertex = 0;
    int cur_index = 0;
    internal_frame_t* frames = NULL;

    u32* indices = NULL;

    char line[1024];
    while(fgets(line, 1024, fp)) {
        if(line[0] == 'h' && line[1] == 'd')
        {
            int result = sscanf(line, "hd %u %u %u", &res->num_vertices, &res->num_indices, &res->num_frames);
            if(result != 3)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(res);
                return NULL;
            }

            res->frames = (frame_t*)malloc(sizeof(frame_t) * res->num_frames);
            frames = (internal_frame_t*)malloc(sizeof(internal_frame_t) * res->num_frames);
            for(u32 i=0; i<res->num_frames; ++i)
            {
                frames[i].vertices = (anim_vertex_data_t*)malloc(sizeof(anim_vertex_data_t) * res->num_vertices);
            }
            indices = (u32*)malloc(sizeof(u32) * res->num_indices);
        }
        else if(line[0] == 'f' && line[1] == 'r')
        {
            cur_vertex = 0;
            cur_frame++;
        }
        else if(line[0] == 'v' && line[1] == 't')
        {
            Vec3 position;
            Vec2 uv;
            Vec3 normal;
            int result = sscanf(line, "vt %f %f %f %f %f %f %f %f", &position.x, &position.y, &position.z,
                                &uv.x, &uv.y, &normal.x, &normal.y, &normal.z);
            if(result != 8)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(res);
                return NULL;
            }

            anim_vertex_data_t vert;
            vert.position = position;
            vert.uv = uv;
            vert.normal = normal;

            frames[cur_frame].vertices[cur_vertex++] = vert;
        }
        else if(line[0] == 'i' && line[1] == 'n')
        {
            u32 id=0;
            int result = sscanf(line, "in %u", &id);
            if(result != 1)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(res);
                return NULL;
            }
            indices[cur_index++] = id;
        }
    }
    fclose(fp);

    //calc next position
    for(u32 i=0; i<res->num_frames; ++i)
    {
        internal_frame_t* frame = &frames[i];
        internal_frame_t* next_frame = NULL;
        if(i >= res->num_frames-1)
        {
            next_frame = &frames[0];
        }
        else
        {
            next_frame = &frames[i+1];
        }
        assert(next_frame);

        for(u32 j=0; j<res->num_vertices; ++j)
        {
            frame->vertices[j].next_position = next_frame->vertices[j].position;
        }
    }

    //create buffers
    int size = res->num_vertices * sizeof(anim_vertex_data_t );
    int stride = sizeof(anim_vertex_data_t);

    GLCall(glGenVertexArrays(1, &res->vao));
    GLCall(glBindVertexArray(res->vao));

    for(u32 i=0; i<res->num_frames; ++i)
    {
        internal_frame_t* internal_frame = &frames[i];
        GLCall(glGenBuffers(1, &res->frames[i].vbo));

        // Generate and populate the buffers with vertex attributes and the indices
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, res->frames[i].vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, internal_frame->vertices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glEnableVertexAttribArray(2));
        GLCall(glEnableVertexAttribArray(3));

        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                                     (const GLvoid *) (offsetof(anim_vertex_data_t , position))));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                                     (const GLvoid *) (offsetof(anim_vertex_data_t , uv))));
        GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
                                     (const GLvoid *) (offsetof(anim_vertex_data_t , normal))));
        GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride,
                                     (const GLvoid *) (offsetof(anim_vertex_data_t , next_position))));

        GLCall(glDisableVertexAttribArray(0));
        GLCall(glDisableVertexAttribArray(1));
        GLCall(glDisableVertexAttribArray(2));
        GLCall(glDisableVertexAttribArray(3));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    GLCall(glGenBuffers(1, &res->ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * res->num_indices, indices, GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    //cleanup
    for(u32 i=0; i<res->num_frames; ++i)
    {
        free(frames[i].vertices);
    }
    free(frames);
    free(indices);

    return res;
}

void anim_destroy(animation_t* anim)
{
    free(anim->frames);
}

void anim_render(animation_t* anim, u32 frame)
{
    GLCall(glBindVertexArray(anim->vao));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, anim->frames[frame].vbo));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, anim->ibo));
    GLCall(glDrawElements(GL_TRIANGLES, anim->num_indices, GL_UNSIGNED_INT, 0));

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}