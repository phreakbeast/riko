#include "tek_mesh.hpp"

#include "tek_opengl.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Mesh* load_mesh(const char* filename);
static Mesh* load_obj(const char* filename);

MeshVertexData mesh_vertex_data_create(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color)
{
    MeshVertexData res;
    res.position = position;
    res.uv = uv;
    res.normal = normal;
    res.color = color;
    return res;
}

void mesh_destroy(Mesh* mesh)
{
    GLCall(glDeleteBuffers(4, mesh->vbo));
    GLCall(glDeleteVertexArrays(1, &mesh->vao));
}

void mesh_create(Mesh* mesh, MeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices, bool calc_normals)
{
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;
    
    //calculating normals
    if(calc_normals)
    {
        /*
        for (u32 i = 0; i < num_indices; i += 3)
        {
            u32 index0 = indices[i];
            u32 index1 = indices[i + 1];
            u32 index2 = indices[i + 2];
            math::Vec3 v1 = vertices[index1].position - vertices[index0].position;
            math::Vec3 v2 = vertices[index2].position - vertices[index0].position;
            math::Vec3 normal = v1.cross(v2);
            normal = normal.normalized();

            vertices[index0].normal = vertices[index0].normal + normal;
            vertices[index1].normal = vertices[index1].normal + normal;
            vertices[index2].normal = vertices[index2].normal + normal;
        }

        for (u32 i = 0; i < num_vertices; i++)
        {
            vertices[i].normal = vertices[i].normal.normalized();
        }
         */
    }
    
    int size = num_vertices * sizeof(MeshVertexData);
    int stride = sizeof(MeshVertexData);
    
    GLCall(glGenVertexArrays(1, &mesh->vao));
    GLCall(glBindVertexArray(mesh->vao));
    
    GLCall(glGenBuffers(4, mesh->vbo));
    
    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(MeshVertexData, position))));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(MeshVertexData, uv))));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(MeshVertexData, normal))));
    GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(MeshVertexData, color))));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * num_indices, indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]);
    
    for(unsigned int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
                              (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(4 + i, 1);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[3]);
    
    for(unsigned int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(8 + i);
        glVertexAttribPointer(8 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
                              (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(8 + i, 1);
    }
    
    mesh->min = vec3_create(0, 0, 0);
    mesh->max = vec3_create(0, 0, 0);
    mesh->center = vec3_create(0, 0, 0);
    
    for(u32 i = 0; i < num_vertices; ++i)
    {
        MeshVertexData* v = &vertices[i];
        if(v->position.x < mesh->min.x)
        {
            mesh->min.x = v->position.x;
        }
        if(v->position.x > mesh->max.x)
        {
            mesh->max.x = v->position.x;
        }
        
        if(v->position.y < mesh->min.y)
        {
            mesh->min.y = v->position.y;
        }
        if(v->position.y > mesh->max.y)
        {
            mesh->max.y = v->position.y;
        }
        
        if(v->position.z < mesh->min.z)
        {
            mesh->min.z = v->position.z;
        }
        if(v->position.z > mesh->max.z)
        {
            mesh->max.z = v->position.z;
        }
    }
    
    mesh->center.x = mesh->max.x - mesh->min.x;
    mesh->center.y = mesh->max.y - mesh->min.y;
    mesh->center.z = mesh->max.z - mesh->min.z;
}

void mesh_render(Mesh* mesh)
{
    GLCall(glBindVertexArray(mesh->vao));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]));

    GLCall(glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0));    

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void mesh_render_instanced(Mesh* mesh, Mat4* mvp_mats, Mat4* world_mats, u32 num_instances)
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4) * num_instances, mvp_mats, GL_DYNAMIC_DRAW));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[3]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4) * num_instances, world_mats, GL_DYNAMIC_DRAW));
    
    GLCall(glBindVertexArray(mesh->vao));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glEnableVertexAttribArray(3));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]));
    
    GLCall(glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                      mesh->num_indices,
                                      GL_UNSIGNED_INT,
                                      0,
                                      num_instances,
                                      0));
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(2));
    GLCall(glDisableVertexAttribArray(3));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}


Mesh* load_obj(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return 0;
    }
    
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    
    u32 num_position = 0;
    u32 num_normals = 0;
    u32 num_uvs = 0;
    u32 num_faces = 0;
    
    char line[1024];
    while(fgets(line, 1024, fp))
    {
        if(line[0] == 'v' && line[1] == ' ')
        {
            num_position++;
        }
        else if(line[0] == 'v' && line[1] == 'n')
        {
            num_normals++;
        }
        else if(line[0] == 'v' && line[1] == 't')
        {
            num_uvs++;
        }
        else if(line[0] == 'f' && line[1] == ' ')
        {
            num_faces++;
        }
    }
    
    Vec3* temp_positions = (Vec3*)malloc(sizeof(Vec3) * num_position);
    Vec3* temp_normals = (Vec3*)malloc(sizeof(Vec3) * num_normals);
    Vec3* temp_uvs = (Vec3*)malloc(sizeof(Vec3) * num_uvs);
    
    MeshVertexData* vertices = (MeshVertexData*)malloc(sizeof(MeshVertexData) * num_faces * 3);
    u32* indices = (u32*)malloc(sizeof(u32) * num_faces * 3);
    
    u32 position_ctr = 0;
    u32 normal_ctr = 0;
    u32 uv_ctr = 0;
    u32 vertex_ctr = 0;
    u32 index_ctr = 0;
    
    fseek(fp, 0, SEEK_SET);
    while(fgets(line, 1024, fp))
    {
        //std::cout << line << std::endl;
        if(line[0] == '\n')
        {
            continue;
        }
        
        if(line[0] == 'v' && line[1] == ' ')
        {
            
            float x, y, z;
            
            int res = sscanf(line, "v %f %f %f", &x, &y, &z);
            if(res < 3)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                return 0;
            }
            
            Vec3 v = {x, y, z};
            temp_positions[position_ctr++] = v;
        }
        else if(line[0] == 'v' && line[1] == 'n')
        {
            float x, y, z;
            
            int res = sscanf(line, "vn %f %f %f", &x, &y, &z);
            if(res < 3)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                return 0;
            }
            
            Vec3 v = {x, y, z};
            temp_normals[normal_ctr++] = v;
        }
        else if(line[0] == 'v' && line[1] == 't')
        {
            float x, y;
            
            int res = sscanf(line, "vt %f %f", &x, &y);
            if(res < 2)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                return 0;
            }
            
            Vec3 v = {x, y, 0};
            temp_uvs[uv_ctr++] = v;
        }
        else if(line[0] == 'f' && line[1] == ' ')
        {
            int v0, t0, n0;
            int v1, t1, n1;
            int v2, t2, n2;
            
            int res = sscanf(&line[0], "f %d/%d/%d %d/%d/%d %d/%d/%d",
                             &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
            if(res < 9)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                return 0;
            }
            
            MeshVertexData vert0;
            vert0.position = temp_positions[v0 - 1];
            vert0.normal = temp_normals[n0 - 1];
            vert0.uv.x = temp_uvs[t0 - 1].x;
            vert0.uv.y = temp_uvs[t0 - 1].y;
            vert0.color = vec4_create(1, 1, 1, 1);
            
            vertices[vertex_ctr] = vert0;
            indices[index_ctr++] = vertex_ctr++;
            
            MeshVertexData vert1;
            vert1.position = temp_positions[v1 - 1];
            vert1.normal = temp_normals[n1 - 1];
            vert1.uv.x = temp_uvs[t1 - 1].x;
            vert1.uv.y = temp_uvs[t1 - 1].y;
            vert1.color = vec4_create(1, 1, 1, 1);
            
            vertices[vertex_ctr] = vert1;
            indices[index_ctr++] = vertex_ctr++;
            
            MeshVertexData vert2;
            vert2.position = temp_positions[v2 - 1];
            vert2.normal = temp_normals[n2 - 1];
            vert2.uv.x = temp_uvs[t2 - 1].x;
            vert2.uv.y = temp_uvs[t2 - 1].y;
            vert2.color = vec4_create(1, 1, 1, 1);
            
            vertices[vertex_ctr] = vert2;
            indices[index_ctr++] = vertex_ctr++;
        }
    }
    
    fclose(fp);
    
    printf("Mesh file proccessed. Creating geometry ...\n");
    
    mesh_create(mesh, vertices, vertex_ctr, indices, index_ctr, false);
    
    free(temp_positions);
    free(temp_normals);
    free(temp_uvs);
    free(vertices);
    free(indices);
    
    return mesh;
}

Mesh* load_mesh(const char* filename)
{
    /*
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return nullptr;
    }

    Mesh *mesh = new Mesh();

    u32 vertex_ctr = 0;
    u32 index_ctr = 0;

    char line[1024];
    while (fgets(line, 1024, fp))
    {
        if (line[0] == 'h' && line[1] == 'd')
        {
            u32 num_vertices, num_indices;

            int result = sscanf(line, "hd %u %u", &num_vertices, &num_indices);
            if (result < 2)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                delete mesh;
                return nullptr;
            }

            mesh->num_vertices = num_vertices;
            mesh->num_indices = num_indices;

            mesh->indices = new u32[num_indices];
            mesh->vertices = new Mesh::VertexData[num_vertices];
        }
        else if (line[0] == 'v' && line[1] == 'x')
        {
            float x, y, z;
            float u, v;
            float nx, ny, nz;

            int result = sscanf(line, "vx %f %f %f %f %f %f %f %f", &x, &y, &z, &u, &v, &nx, &ny, &nz);
            if (result < 8)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                delete mesh;
                return nullptr;
            }

            Mesh::VertexData vert = {x, y, z, u, v, nx, ny, nz, 1, 1, 1, 1};
            mesh->vertices[vertex_ctr++] = vert;
        }
        else if (line[0] == 'i' && line[1] == 'd')
        {
            u32 idx;
            int result = sscanf(line, "id %u", &idx);
            if (result < 1)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                delete mesh;
                return nullptr;
            }

            mesh->indices[index_ctr++] = idx;
        }
    }

    fclose(fp);

    mesh->create(mesh->vertices, vertex_ctr, mesh->indices, index_ctr, 0);

    return mesh;
     */
    return NULL;
}

Mesh* mesh_load(const char* filename)
{
    printf("Loading mesh %s\n", filename);
    
    Mesh* res = NULL;
    const char* ext = get_file_extension(filename);
    if((strcmp(ext, "obj") == 0) || (strcmp(ext, "OBJ") == 0))
    {
        res = load_obj(filename);
    }
    else if((strcmp(ext, "mesh") == 0) || (strcmp(ext, "MESH") == 0))
    {
        res = load_mesh(filename);
    }
    if(!res)
    {
        return NULL;
    }
    
    return res;
}

