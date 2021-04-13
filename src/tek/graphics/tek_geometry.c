#include "tek_geometry.h"

#include "tek_rect.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void mesh_buffer_init(MeshBuffer* buffer)
{
    buffer->res_vertices = NULL;
    buffer->res_indices = NULL;
    buffer->num_vertices = 0;
    buffer->num_indices = 0;
}

void mesh_buffer_destroy(MeshBuffer* buffer)
{
    free(buffer->res_vertices);
    free(buffer->res_indices);
}

void mesh_buffer_add_plane(MeshBuffer* buffer, float width, float depth, Color color)
{
    MeshVertexData vertices[4];
    u32 indices[6];
    
    float w = width / 2.0f;
    float d = depth / 2.0f;
    Vec4 col = color_to_vec4(color);
    
    vertices[0] = mesh_vertex_data_create(vec3_create(-w, 0, -d), vec2_create(0, 1), vec3_create(0, 1, 0), col);
    vertices[1] = mesh_vertex_data_create(vec3_create(-w, 0, d),  vec2_create(0, 0), vec3_create(0, 1, 0), col);
    vertices[2] = mesh_vertex_data_create(vec3_create(w, 0, d),   vec2_create(1, 0), vec3_create(0, 1, 0), col);
    vertices[3] = mesh_vertex_data_create(vec3_create(w, 0, -d),  vec2_create(1, 1), vec3_create(0, 1, 0), col);
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    
    mesh_buffer_add_vertices(buffer, vertices, 4, indices, 6);
}

void mesh_buffer_add_box(MeshBuffer* buffer, float width, float height, float depth, Color color, Vec3 offset)
{
    MeshVertexData vertices[24];
    u32 indices[36];
    
    float w = width / 2.0f;
    float d = depth / 2.0f;
    float h = height;
    
    float xo = offset.x;
    float yo = offset.y;
    float zo = offset.z;
    
    Vec4 col = color_to_vec4(color);
    
    //front
    vertices[0] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, d + zo), vec2_create(0, 1), vec3_create(0, 0, 1), col);
    vertices[1] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, d + zo), vec2_create(0, 0), vec3_create(0, 0, 1), col);
    vertices[2] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, d + zo),  vec2_create(1, 0), vec3_create(0, 0, 1), col);
    vertices[3] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, d + zo),  vec2_create(1, 1), vec3_create(0, 0, 1), col);
    
    //left
    vertices[4] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, -d + zo), vec2_create(0, 1), vec3_create(-1, 0, 0), col);
    vertices[5] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, -d + zo), vec2_create(0, 0), vec3_create(-1, 0, 0), col);
    vertices[6] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, d + zo),  vec2_create(1, 0), vec3_create(-1, 0, 0), col);
    vertices[7] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, d + zo),  vec2_create(1, 1), vec3_create(-1, 0, 0), col);
    
    //right
    vertices[8] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, d + zo),  vec2_create(0, 1), vec3_create(1, 0, 0), col);
    vertices[9] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, d + zo),  vec2_create(0, 0), vec3_create(1, 0, 0), col);
    vertices[10] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, -d + zo), vec2_create(1, 0), vec3_create(1, 0, 0), col);
    vertices[11] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, -d + zo), vec2_create(1, 1), vec3_create(1, 0, 0), col);
    
    //back
    vertices[12] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, -d + zo),  vec2_create(0, 1), vec3_create(0, 0, -1), col);
    vertices[13] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, -d + zo),  vec2_create(0, 0), vec3_create(0, 0, -1), col);
    vertices[14] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, -d + zo), vec2_create(1, 0), vec3_create(0, 0, -1), col);
    vertices[15] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, -d + zo), vec2_create(1, 1), vec3_create(0, 0, -1), col);
    
    //top
    vertices[16] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, -d + zo), vec2_create(0, 1), vec3_create(0, 1, 0), col);
    vertices[17] = mesh_vertex_data_create(vec3_create(-w + xo, h + yo, d + zo),  vec2_create(0, 0), vec3_create(0, 1, 0), col);
    vertices[18] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, d + zo),   vec2_create(1, 0), vec3_create(0, 1, 0), col);
    vertices[19] = mesh_vertex_data_create(vec3_create(w + xo, h + yo, -d + zo),  vec2_create(1, 1), vec3_create(0, 1, 0), col);
    
    //bottom
    vertices[20] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, d + zo),  vec2_create(0, 1), vec3_create(0, -1, 0), col);
    vertices[21] = mesh_vertex_data_create(vec3_create(-w + xo, 0 + yo, -d + zo), vec2_create(0, 0), vec3_create(0, -1, 0), col);
    vertices[22] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, -d + zo),  vec2_create(1, 0), vec3_create(0, -1, 0), col);
    vertices[23] = mesh_vertex_data_create(vec3_create(w + xo, 0 + yo, d + zo),   vec2_create(1, 1), vec3_create(0, -1, 0), col);
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    
    indices[6] = 4;
    indices[7] = 5;
    indices[8] = 6;
    indices[9] = 4;
    indices[10] = 6;
    indices[11] = 7;
    
    indices[12] = 8;
    indices[13] = 9;
    indices[14] = 10;
    indices[15] = 8;
    indices[16] = 10;
    indices[17] = 11;
    
    indices[18] = 12;
    indices[19] = 13;
    indices[20] = 14;
    indices[21] = 12;
    indices[22] = 14;
    indices[23] = 15;
    
    indices[24] = 16;
    indices[25] = 17;
    indices[26] = 18;
    indices[27] = 16;
    indices[28] = 18;
    indices[29] = 19;
    
    indices[30] = 20;
    indices[31] = 21;
    indices[32] = 22;
    indices[33] = 20;
    indices[34] = 22;
    indices[35] = 23;
    
    mesh_buffer_add_vertices(buffer, vertices, 24, indices, 36);
}

void mesh_buffer_add_cylinder(MeshBuffer* buffer, float radius, int num_slices, float height, bool with_positions, bool with_uv, bool with_normals)
{
    /*
    // Calculate and cache numbers of vertices
    int _numVerticesSide = (num_slices + 1) * 2;
    int _numVerticesTopBottom = num_slices + 2;
    int _numVerticesTotal = _numVerticesSide + _numVerticesTopBottom * 2;
    
    // Pre-calculate sines / cosines for given number of slices
    const auto sliceAngleStep = 2.0f * M_PI / static_cast<float>(num_slices);
    auto currentSliceAngle = 0.0f;
    std::vector<float> sines, cosines;
    for(auto i = 0; i <= num_slices; i++)
    {
        sines.push_back(sinf(currentSliceAngle));
        cosines.push_back(cosf(currentSliceAngle));
        
        // Update slice angle
        currentSliceAngle += sliceAngleStep;
    }
    
    if(with_positions)
    {
        // Pre-calculate X and Z coordinates
        std::vector<float> x;
        std::vector<float> z;
        for(auto i = 0; i <= num_slices; i++)
        {
            x.push_back(cosines[i] * radius);
            z.push_back(sines[i] * radius);
        }
        
        // Add cylinder side vertices
        for(auto i = 0; i <= num_slices; i++)
        {
            const auto topPosition = Vec3(x[i], height / 2.0f, z[i]);
            const auto bottomPosition = Vec3(x[i], -height / 2.0f, z[i]);
            //_vbo.addRawData(&topPosition, sizeof(glm::vec3));
            //_vbo.addRawData(&bottomPosition, sizeof(glm::vec3));
        }
        
        // Add top cylinder cover
        Vec3 topCenterPosition = Vec3(0.0f, height / 2.0f, 0.0f);
        //_vbo.addRawData(&topCenterPosition, sizeof(glm::vec3));
        for(auto i = 0; i <= num_slices; i++)
        {
            const auto topPosition = Vec3(x[i], height / 2.0f, z[i]);
            //_vbo.addRawData(&topPosition, sizeof(glm::vec3));
        }
        
        // Add bottom cylinder cover
        Vec3 bottomCenterPosition = Vec3(0.0f, -height / 2.0f, 0.0f);
        //_vbo.addRawData(&bottomCenterPosition, sizeof(glm::vec3));
        for(auto i = 0; i <= num_slices; i++)
        {
            const auto bottomPosition = Vec3(x[i], -height / 2.0f, -z[i]);
            //_vbo.addRawData(&bottomPosition, sizeof(glm::vec3));
        }
    }
    
    if(with_uv)
    {
        // Pre-calculate step size in texture coordinate U
        // I have decided to map the texture twice around cylinder, looks fine
        const auto sliceTextureStepU = 2.0f / static_cast<float>(num_slices);
        
        auto currentSliceTexCoordU = 0.0f;
        for(auto i = 0; i <= num_slices; i++)
        {
            //_vbo.addData(glm::vec2(currentSliceTexCoordU, 1.0f));
            //_vbo.addData(glm::vec2(currentSliceTexCoordU, 0.0f));
            
            // Update texture coordinate of current slice
            currentSliceTexCoordU += sliceTextureStepU;
        }
        
        // Generate circle texture coordinates for cylinder top cover
        Vec2 topBottomCenterTexCoord = Vec2(0.5f, 0.5f);
        //_vbo.addData(topBottomCenterTexCoord);
        for(auto i = 0; i <= num_slices; i++)
        {
            //_vbo.addData(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y + cosines[i] * 0.5f));
        }
        
        // Generate circle texture coordinates for cylinder bottom cover
        //_vbo.addData(topBottomCenterTexCoord);
        for(auto i = 0; i <= num_slices; i++)
        {
            //_vbo.addData(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y - cosines[i] * 0.5f));
        }
    }
    
    if(with_normals)
    {
        for(auto i = 0; i <= num_slices; i++)
        {
            //_vbo.addData(glm::vec3(cosines[i], 0.0f, sines[i]), 2);
        }
        
        // Add normal for every vertex of cylinder top cover
        //_vbo.addData(glm::vec3(0.0f, 1.0f, 0.0f), _numVerticesTopBottom);
        
        // Add normal for every vertex of cylinder bottom cover
        //_vbo.addData(glm::vec3(0.0f, -1.0f, 0.0f), _numVerticesTopBottom);
    }
    */
    
    // Finally upload data to the GPU
    //_vbo.bindVBO();
    //_vbo.uploadDataToGPU(GL_STATIC_DRAW);
    //setVertexAttributesPointers(_numVerticesTotal);
    
    /*
    std::vector<MeshVertexData> vertices;
    std::vector<u32> indices;

    std::vector<Vec2> points;
    Vec4 col = color.to_vec4();

    float seg = 360.0f / segment;
    for (int i = 0; i < segment; ++i)
    {
        float x, y;
        float angle = seg * i;

        math_get_circle_coord(rad, 0, 0, angle, &x, &y);
        Vec2 v = { x, y };
        points.push_back(v);
    }

    for (int i = 0; i < segment; ++i)
    {
        float x1 = points[i].x;
        float y1 = points[i].y;

        float x2, y2;
        if (i == segment - 1)
        {
            x2 = points[0].x;
            y2 = points[0].y;
        }
        else
        {
            x2 = points[i + 1].x;
            y2 = points[i + 1].y;
        }

        float fixed_uv_x1 = ((float)i / segment) * uv_x;
        float fixed_uv_x2;

        if (i == segment - 1)
        {
            fixed_uv_x2 = 1 * uv_x;
        }
        else
        {
            fixed_uv_x2 = ((float)(i + 1) / segment) * uv_x;
        }

        Vec3 p0(x2, h, y2);
        Vec3 p1(x2, 0, y2);
        Vec3 p2(x1, 0, y1);
        Vec3 p3(x1, h, y1);

        Vec3 n1 = vec3_normalized(math_calc_normals(p0, p1, p2));
        Vec3 n2 = vec3_normalized(math_calc_normals(p0, p2, p3));

        u32 vert_id = vertices.size();

        vertices.push_back(mesh_vertex(p0, Vec2(fixed_uv_x2, 0), n1, Vec4(col.x, col.y, col.z, col.w))); //0
        vertices.push_back(mesh_vertex(p1, Vec2(fixed_uv_x2, uv_y), n2, Vec4(col.x, col.y, col.z, col.w))); //1
        vertices.push_back(mesh_vertex(p2, Vec2(fixed_uv_x1, uv_y), n1, Vec4(col.x, col.y, col.z, col.w))); //2
        vertices.push_back(mesh_vertex(p3, Vec2(fixed_uv_x1, 0), n1, Vec4(col.x, col.y, col.z, col.w))); //3

        vertices.push_back(mesh_vertex(p0, Vec2(fixed_uv_x2, 0), n1, Vec4(col.x, col.y, col.z, col.w))); //0
        vertices.push_back(mesh_vertex(p3, Vec2(fixed_uv_x1, 0), n1, Vec4(col.x, col.y, col.z, col.w))); //3
        vertices.push_back(mesh_vertex(0, h, 0, 0, 0, 0, 1, 0, col.x, col.y, col.z, col.w)); //4

        vertices.push_back(mesh_vertex(p1, Vec2(fixed_uv_x2, uv_y), n2, Vec4(col.x, col.y, col.z, col.w))); //1
        vertices.push_back(mesh_vertex(p2, Vec2(fixed_uv_x1, uv_y), n1, Vec4(col.x, col.y, col.z, col.w))); //2
        vertices.push_back(mesh_vertex(0, 0, 0, 0, 0, 0, -1, 0, col.x, col.y, col.z, col.w)); //5

        indices.push_back(vert_id + 0);
        indices.push_back(vert_id + 1);
        indices.push_back(vert_id + 2);
        indices.push_back(vert_id + 0);
        indices.push_back(vert_id + 2);
        indices.push_back(vert_id + 3);

        indices.push_back(vert_id + 4);
        indices.push_back(vert_id + 5);
        indices.push_back(vert_id + 6);

        indices.push_back(vert_id + 7);
        indices.push_back(vert_id + 8);
        indices.push_back(vert_id + 9);
    }

    add_vertices(vertices, indices);
     */
}

void mesh_buffer_add_sphere(MeshBuffer* buffer, float rad, int rings, int sectors, int inverted, Color color)
{    
    u32 num_vertices = rings * sectors;
    MeshVertexData* vertices = malloc(sizeof(MeshVertexData) * num_vertices);
    assert(vertices);

    u32 num_indices = rings * sectors * 6;
    u32* indices = malloc(sizeof(u32) * num_indices);
    assert(indices);

    const float R = 1.0f / (float) (rings - 1);
    const float S = 1.0f / (float) (sectors - 1);
    Vec4 col = color_to_vec4(color);

    u32 vertex_ctr = 0;
    for (int r = 0; r < rings; r++)
    {
        for (int s = 0; s < sectors; s++)
        {
            float const y = sinf(-M_PI_2 + M_PI * r * R);
            float const x = cosf(2 * M_PI * s * S) * sinf(M_PI * r * R);
            float const z = sinf(2 * M_PI * s * S) * sinf(M_PI * r * R);


            float u = s * S;
            float v = r * R;

            float px = x * rad;
            float py = y * rad;
            float pz = z * rad;

            float nx = x;
            float ny = y;
            float nz = z;

            vertices[vertex_ctr++] = mesh_vertex_data_create(vec3_create(px, py, pz), vec2_create(u, v), vec3_create(nx, ny, nz), vec4_create(col.x, col.y, col.z, col.w));
        }
    }

    u32 indices_ctr = 0;
    for (int r = 0; r < rings; r++)
    {
        for (int s = 0; s < sectors; s++)
        {
            if (inverted)
            {
                indices[indices_ctr++] = ((r * sectors + s)); //0
                indices[indices_ctr++] = ((r * sectors + (s + 1))); //1
                indices[indices_ctr++] = (((r + 1) * sectors + (s + 1))); //2
                indices[indices_ctr++] = ((r * sectors + s)); //0
                indices[indices_ctr++] = (((r + 1) * sectors + (s + 1))); //2
                indices[indices_ctr++] = (((r + 1) * sectors + s)); //3
            }
            else
            {
                indices[indices_ctr++] = ((r * sectors + s)); //0
                indices[indices_ctr++] = (((r + 1) * sectors + (s + 1))); //2
                indices[indices_ctr++] = ((r * sectors + (s + 1))); //1
                indices[indices_ctr++] = ((r * sectors + s)); //0
                indices[indices_ctr++] = (((r + 1) * sectors + s)); //3
                indices[indices_ctr++] = (((r + 1) * sectors + (s + 1))); //2
            }
        }
    }

    mesh_buffer_add_vertices(buffer,vertices,num_vertices, indices,num_indices);
}

void mesh_buffer_add_pyramid(MeshBuffer* buffer, Vec3 position, float width, float height, Color color)
{   
    MeshVertexData vertices[5];
    u32 indices[18];

    Vec3 p0 = vec3_create(position.x - width / 2, position.y, position.z - width / 2);
    Vec3 p1 = vec3_create(position.x - width / 2, position.y, position.z + width / 2);
    Vec3 p2 = vec3_create(position.x + width / 2, position.y, position.z + width / 2);
    Vec3 p3 = vec3_create(position.x + width / 2, position.y, position.z - width / 2);

    Vec3 p4 = vec3_create(position.x, position.y + height, position.z);

    Vec4 col = color_to_vec4(color);

    vertices[0] = mesh_vertex_data_create(p0, vec2_create(0, 0), vec3_create(0, 0, 0), col);
    vertices[1] = mesh_vertex_data_create(p1, vec2_create(0, 0), vec3_create(0, 0, 0), col);
    vertices[2] = mesh_vertex_data_create(p2, vec2_create(0, 0), vec3_create(0, 0, 0), col);
    vertices[3] = mesh_vertex_data_create(p3, vec2_create(0, 0), vec3_create(0, 0, 0), col);

    vertices[4] = mesh_vertex_data_create(p4, vec2_create(0, 0), vec3_create(0, 0, 0), col);

    indices[0] = (4);
    indices[1] = (1);
    indices[2] = (2);

    indices[3] = (4);
    indices[4] = (2);
    indices[5] = (3);

    indices[6] = (4);
    indices[7] = (3);
    indices[8] = (0);

    indices[9] = (4);
    indices[10] = (0);
    indices[11] = (1);

    indices[12] = (1);
    indices[13] = (0);
    indices[14] = (3);

    indices[15] = (1);
    indices[16] = (3);
    indices[17] = (2);

    mesh_buffer_add_vertices(buffer, vertices, 5, indices, 18);
}


bool load_obj(const char* filename, MeshVertexData** vertices, u32* num_vertices, u32** indices, u32* num_indices)
{
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return false;
    }
    
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
    
    Vec3* temp_positions = malloc(sizeof(Vec3) * num_position);
    assert(temp_positions);
    Vec3* temp_normals = malloc(sizeof(Vec3) * num_normals);
    assert(temp_normals);
    Vec3* temp_uvs = malloc(sizeof(Vec3) * num_uvs);
    assert(temp_uvs);
    
    *vertices = malloc(sizeof(MeshVertexData) * num_faces * 3);
    assert(vertices);
    *indices = malloc(sizeof(u32) * num_faces * 3);
    assert(indices);
    *num_vertices = num_faces * 3;
    *num_indices = num_faces * 3;
    
    u32 position_ctr = 0;
    u32 normal_ctr = 0;
    u32 uv_ctr = 0;
    u32 vertex_ctr = 0;
    u32 index_ctr = 0;
    u32 array_ctr = 0;
    
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
                return false;
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
                return false;
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
                return false;
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
                return false;
            }
            
            MeshVertexData vert0;
            vert0.position = temp_positions[v0 - 1];
            vert0.normal = temp_normals[n0 - 1];
            vert0.uv.x = temp_uvs[t0 - 1].x;
            vert0.uv.y = temp_uvs[t0 - 1].y;
            vert0.color = vec4_create(1, 1, 1, 1);
            
            (*vertices)[array_ctr] = vert0;
            (*indices)[array_ctr] = vertex_ctr++;
            array_ctr++;
            
            MeshVertexData vert1;
            vert1.position = temp_positions[v1 - 1];
            vert1.normal = temp_normals[n1 - 1];
            vert1.uv.x = temp_uvs[t1 - 1].x;
            vert1.uv.y = temp_uvs[t1 - 1].y;
            vert1.color = vec4_create(1, 1, 1, 1);
            
            (*vertices)[array_ctr] = vert1;
            (*indices)[array_ctr] = vertex_ctr++;
            array_ctr++;
            
            MeshVertexData vert2;
            vert2.position = temp_positions[v2 - 1];
            vert2.normal = temp_normals[n2 - 1];
            vert2.uv.x = temp_uvs[t2 - 1].x;
            vert2.uv.y = temp_uvs[t2 - 1].y;
            vert2.color = vec4_create(1, 1, 1, 1);
            
            (*vertices)[array_ctr] = vert2;
            (*indices)[array_ctr]= vertex_ctr++;
            array_ctr++;
        }
    }
    
    fclose(fp);
    
    free(temp_positions);
    free(temp_normals);
    free(temp_uvs);
        
    return true;
}

bool mesh_buffer_load_mesh(MeshBuffer* buffer, const char* filename)
{
    MeshVertexData* vertices = NULL;
    u32* indices = NULL;
    u32 num_vertices = 0;
    u32 num_indices = 0;
    
    if(!load_obj(filename, &vertices, &num_vertices, &indices, &num_indices))
    {
        return false;
    }
    
    mesh_buffer_add_vertices(buffer, vertices, num_vertices, indices, num_indices);
    
    return true;
}

void mesh_buffer_add_vertices(MeshBuffer* buffer, MeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
    if (buffer->num_vertices == 0 && buffer->num_indices == 0)
    {
        buffer->num_vertices = num_vertices;
        buffer->num_indices = num_indices;
        buffer->res_vertices = malloc(sizeof(MeshVertexData) * num_vertices);
        assert(buffer->res_vertices);
        buffer->res_indices = malloc(sizeof(u32) * num_indices);
        assert(buffer->res_indices);

        memcpy(buffer->res_vertices, vertices, sizeof(MeshVertexData) * num_vertices);
        memcpy(buffer->res_indices, indices, sizeof(u32) * num_indices);
    }
    else
    {
        u32 first_vertex = buffer->num_vertices;
        u32 first_index = buffer->num_indices;

        buffer->num_vertices += num_vertices;
        buffer->num_indices += num_indices;
        buffer->res_vertices = realloc(buffer->res_vertices, sizeof(MeshVertexData) * buffer->num_vertices);
        assert(buffer->res_vertices);
        buffer->res_indices = realloc(buffer->res_indices, sizeof(u32) * buffer->num_indices);
        assert(buffer->res_indices);

        memcpy(&buffer->res_vertices[first_vertex], vertices, sizeof(MeshVertexData) * num_vertices);
        memcpy(&buffer->res_indices[first_index], indices, sizeof(u32) * num_indices);
    }    
}

PickMesh* mesh_buffer_to_pickmesh(MeshBuffer* buffer)
{
    PickVertexData* vertices = malloc(sizeof(PickVertexData) * buffer->num_vertices);
    assert(vertices);
    for(u32 i=0; i< buffer->num_vertices;++i)
    {
        MeshVertexData vert = buffer->res_vertices[i];
        PickVertexData v;
        v.position = vert.position;
        v.color.x = vert.color.x;
        v.color.y = vert.color.y;
        v.color.z = vert.color.z;
        
        vertices[i] = v;
    }
    
    PickMesh* res = pick_mesh_create(&vertices[0], buffer->num_vertices, &buffer->res_indices[0], buffer->num_indices);
    
    return res;
}


Mesh* mesh_buffer_to_mesh(MeshBuffer* buffer)
{
    Mesh* res = malloc(sizeof(Mesh));
    mesh_create(res, &buffer->res_vertices[0], buffer->num_vertices, &buffer->res_indices[0], buffer->num_indices, false);
    return res;
}
