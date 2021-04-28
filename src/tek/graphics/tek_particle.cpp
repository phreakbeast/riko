#include "tek_particle.hpp"

#include "tek_opengl.hpp"
#include "tek_shader.hpp"
#include <assert.h>
#include <stdlib.h>

ParticleEmitter emitter_create(EmitterType type) 
{

    ParticleEmitter res;
    res.type = type;
    res.direction = vec3_create(0, 1, 0);
    res.line_start = vec3_create(0, 0, 0);
    res.line_end = vec3_create(0, 0, 0);
    res.radius = 1.0f;
    res.width = 1.0f;
    res.depth = 1.0f;
    res.height = 1.0f;
    return res;
}

Vec3 emitter_get_point(ParticleEmitter* emitter, bool inside)
{
    switch (emitter->type)
    {
        case EMITTER_POINT:
            return emitter->origin;
        case EMITTER_LINE:
        {
            float t = math_get_random_float(0, 1);
            return vec3_lerp(emitter->line_start,emitter->line_end, t);
        }
            break;
        case EMITTER_CIRCLE:
            {
            if (inside) {
                float a = math_get_random_float(0, 1);
                float b = math_get_random_float(0, 1);
                return vec3_create(b * emitter->radius * cosf(2 * M_PI * a / b), 0, b * emitter->radius * sinf(2 * M_PI * a / b));
            }
        }
            break;
        case EMITTER_SPHERE: {
            if (inside) {

            } else {
                float theta = 2 * M_PI * math_get_random_float(0, 1);
                float phi = M_PI * math_get_random_float(0, 1);
                float x = sinf(phi) * cosf(theta);
                float y = sinf(phi) * sinf(theta);
                float z = cosf(phi);
                return vec3_create(x, y, z);
            }
        }
            break;
        case EMITTER_SQUARE: {
            if (inside) {
                float x = math_get_random_float(-emitter->width / 2.0f, emitter->width / 2.0f);
                float y = math_get_random_float(-emitter->height / 2.0f, emitter->height / 2.0f);
                return vec3_create(x, 0, y);
            } else {

            }
        }
            break;
        case EMITTER_BOX: {
            if (inside) {
                float x = math_get_random_float(-emitter->width / 2.0f, emitter->width / 2.0f);
                float y = math_get_random_float(-emitter->height / 2.0f,emitter->height / 2.0f);
                float z = math_get_random_float(-emitter->depth / 2.0f, emitter->depth / 2.0f);
                return vec3_create(x, y, z);
            } else {

            }
        }
            break;
        default:
            break;
    }
    return emitter->origin;
}

void particle_destroy(ParticleSystem* ps)
{
    texture_destroy(ps->texture);
    free(ps->texture);
    free(ps->uvs);
    GLCall(glDeleteBuffers(1, &ps->vbo));
    GLCall(glDeleteVertexArrays(1, &ps->vao));
}

void particle_update(ParticleSystem* ps, float delta, Vec3 camera_pos)
{
    //update particles
    for (u32 i = 0; i < ps->num_particles; ++i) {
        ParticleVertexData *p = &ps->vertices[i];

        p->life -= delta;
        if (p->life > 0.0f)
        {
            p->position = vec3_mul(vec3_add(p->position, p->direction), ps->speed * delta);
            p->position.y -= ps->gravity;
            float t = p->life / ps->life;
            p->color = vec4_lerp(ps->color_end,ps->color_start, t);
            p->size = math_lerp(ps->size_end, ps->size_start, t);

            if (ps->num_uvs > 0)
            {
                u32 uv_id = (u32) math_lerp((float)ps->num_uvs, 0, t);
                if (uv_id >= ps->num_uvs - 1)
                {
                    uv_id = ps->num_uvs - 1;
                }

                p->uv_rect = rect_to_vec4(ps->uvs[uv_id]);
            } else {
                p->uv_rect = vec4_create(0, 0, 1, 1);
            }
        }
        p->dist_to_cam = vec3_distance(camera_pos, p->position);
    }

    //sort particles
    //std::sort(&vertices[0], &vertices[num_particles - 1]);

    ParticleVertexData *buffer;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ps->vbo));
    GLCall(buffer = (ParticleVertexData *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

    for (u32 i = 0; i < ps->num_particles; ++i)
    {
        ParticleVertexData *p = &ps->vertices[i];

        buffer[i].position = p->position;
        buffer[i].color = p->color;
        buffer[i].uv_rect = p->uv_rect;
        buffer[i].size = p->size;
        buffer[i].life = p->life;
        buffer[i].direction = p->direction;
        buffer[i].dist_to_cam = p->dist_to_cam;
    }

    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}

void particle_render(ParticleSystem* ps)
{
    GLCall(glBindVertexArray(ps->vao));

    glBindBuffer(GL_ARRAY_BUFFER, ps->vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glDrawArrays(GL_POINTS, 0, ps->num_particles);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLCall(glBindVertexArray(0));
}

ParticleSystem* particle_load(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }

    ParticleSystem *ps = (ParticleSystem*)malloc(sizeof(ParticleSystem));

    u32 rect_ctr = 0;

    char line[1024];
    while (fgets(line, 1024, fp))
    {
        if (line[0] == 'h' && line[1] == 'd')
        {
            u32 num;
            float sx, sy, sz;
            float speed;
            float life;
            float gravity;

            int res = sscanf(line, "hd %u %f %f %f %f %f %f", &num, &sx, &sy, &sz, &speed, &life, &gravity);
            if (res < 7)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(ps);
                return NULL;
            }
            //ps->particles = new Particle[num]();
            //ps->vertices.reserve(num);
            ps->num_particles = num;
            ps->gravity = gravity;
            ps->spread = vec3_create(sx, sy, sz);
            ps->speed = speed;
            ps->life = life;
        }
        else if (line[0] == 'e' && line[1] == 'm')
        {
            float ox, oy, oz;
            float dx, dy, dz;
            float lsx, lsy, lsz;
            float lex, ley, lez;
            float radius;
            float w, h, d;
            char type[32];

            int res = sscanf(line, "em %s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
                             type, &ox, &oy, &oz, &dx, &dy, &dz, &lsx, &lsy, &lsz, &lex, &ley, &lez, &radius, &w,
                             &h, &d);
            if (res < 17)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(ps);
                return NULL;
            }

            if (strcmp(type,"point")==0)
            {
                ps->emitter.type = EMITTER_POINT;
                ps->emitter.origin = vec3_create(ox, oy, oz);
                ps->emitter.direction = vec3_create(dx, dy, dz);
            }
            else if (strcmp(type, "line")==0)
            {
                ps->emitter.type = EMITTER_LINE;
                ps->emitter.line_start = vec3_create(lsx, lsy, lsz);
                ps->emitter.line_end = vec3_create(lex, ley, lez);
            }
            else if (strcmp(type, "circle")==0)
            {
                ps->emitter.type = EMITTER_CIRCLE;
                ps->emitter.origin = vec3_create(ox, oy, oz);
                ps->emitter.radius = radius;
            }
            else if (strcmp(type, "sphere")==0)
            {
                ps->emitter.type = EMITTER_SPHERE;
                ps->emitter.origin = vec3_create(ox, oy, oz);
                ps->emitter.radius = radius;
            }
            else if (strcmp(type, "square")==0)
            {
                ps->emitter.type = EMITTER_SQUARE;
                ps->emitter.origin = vec3_create(ox, oy, oz);
                ps->emitter.width = w;
                ps->emitter.height = h;
            }
            else if (strcmp(type, "box")==0)
            {
                ps->emitter.type = EMITTER_BOX;
                ps->emitter.origin = vec3_create(ox, oy, oz);
                ps->emitter.width = w;
                ps->emitter.height = h;
                ps->emitter.depth = d;
            }

            ps->emitter.origin = vec3_create(ox, oy, oz);
        }
        else if (line[0] == 'p' && line[1] == 't')
        {
            int sx, sy, sz, sa;
            int ex, ey, ez, ea;
            float ss, es;
            int res = sscanf(line, "pt %d %d %d %d %d %d %d %d %f %f", &sx, &sy, &sz, &sa, &ex, &ey, &ez, &ea, &ss,
                             &es);
            if (res < 10)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(ps);
                return NULL;
            }
            ps->color_start = color_to_vec4(color_create_alpha(sx, sy, sz, sa));
            ps->color_end = color_to_vec4(color_create_alpha(ex, ey, ez, ea));
            ps->size_start = ss;
            ps->size_end = es;
        }
        else if (line[0] == 't' && line[1] == 'x')
        {
            u32 num;
            char name[256];

            int res = sscanf(line, "tx %s %u", name, &num);
            if (res < 2)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(ps);
                return NULL;
            }

            char path[300];
            sprintf(path,"data/%s",name);
            ps->texture = texture_load(path, TEXTURE_FILTER_LINEAR);

            ps->uvs = (Rect*)malloc(sizeof(Rect) * num);
            ps->num_uvs = num;
        }
        else if (line[0] == 'r' && line[1] == 'c')
        {
            u32 x, y, w, h;

            int res = sscanf(line, "rc %u %u %u %u", &x, &y, &w, &h);
            if (res < 3)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                free(ps);
                return NULL;
            }

            assert(ps->texture);
            u32 tw = ps->texture->width;
            u32 th = ps->texture->height;
            ps->uvs[rect_ctr++] = rect_uv(x, y, w, h, tw, th);
        }
    }

    fclose(fp);

    for (u32 i = 0; i < ps->num_particles; ++i)
    {
        ParticleVertexData v;
        v.life = ps->life;

        v.position = emitter_get_point(&ps->emitter,true);
        float rdx = math_get_random_float(-ps->spread.x, ps->spread.x);
        float rdy = math_get_random_float(-ps->spread.y, ps->spread.y);
        float rdz = math_get_random_float(-ps->spread.z, ps->spread.x);
        v.direction = vec3_add(ps->emitter.direction, vec3_create(rdx, rdy, rdz));

        v.color = ps->color_start;
        if (ps->num_uvs > 0) 
        {
            v.uv_rect = rect_to_vec4(ps->uvs[0]);
        } 
        else 
        {
            v.uv_rect = vec4_create(0, 0, 1, 1);
        }
        v.size = ps->size_start;
        v.dist_to_cam = 0;
        //ps->vertices.push_back(v);
    }

    GLCall(glGenVertexArrays(1, &ps->vao));
    GLCall(glBindVertexArray(ps->vao));

    int stride = sizeof(ParticleVertexData);

    glGenBuffers(1, &ps->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ps->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertexData) * ps->num_particles, &ps->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                                 (const GLvoid *) (offsetof(ParticleVertexData, position))));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                                 (const GLvoid *) (offsetof(ParticleVertexData, color))));
    GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride,
                                 (const GLvoid *) (offsetof(ParticleVertexData, uv_rect))));
    GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride,
                                 (const GLvoid *) (offsetof(ParticleVertexData, size))));
    GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride,
                                 (const GLvoid *) (offsetof(ParticleVertexData, life))));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLCall(glBindVertexArray(0));

    return ps;
}
