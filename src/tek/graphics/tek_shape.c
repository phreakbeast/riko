#include "tek_shape.h"

#include "tek_opengl.h"
#include "tek_shader.h"
#include <stdlib.h>

ShapeVertexData shape_vertex_data_create(Vec3 position, Vec3 color)
{
    ShapeVertexData res;
    res.position = position;
    res.color = color;
    return res;
}

void shape_destroy(Shape* shape)
{
    GLCall(glDeleteBuffers(1, &shape->vbo));
    GLCall(glDeleteBuffers(1, &shape->ibo));
    GLCall(glDeleteVertexArrays(1, &shape->vao));
}

void shape_create(Shape* shape, ShapeVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices)
{
    shape->num_vertices = num_vertices;
    shape->num_indices = num_indices;

    int size = num_vertices * sizeof(ShapeVertexData);

    GLCall(glGenVertexArrays(1, &shape->vao));
    GLCall(glBindVertexArray(shape->vao));

    GLCall(glGenBuffers(1, &shape->vbo));
    GLCall(glGenBuffers(1, &shape->ibo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, shape->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));

    int stride = sizeof(ShapeVertexData);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof(ShapeVertexData, position))));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (offsetof(ShapeVertexData, color))));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(u32), indices, GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void shape_render(Shape* shape)
{
    GLCall(glBindVertexArray(shape->vao));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ibo));
    GLCall(glDrawElements(GL_LINES, shape->num_indices, GL_UNSIGNED_INT, 0));

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDisableVertexAttribArray(1));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

Shape* shape_create_line(Vec3 p0, Vec3 p1, Vec3 color)
{
    ShapeVertexData vertices[2];
    u32 indices[2];

    ShapeVertexData v0 = {p0, color};
    ShapeVertexData v1 = {p1, color};

    vertices[0] = v0;
    vertices[1] = v1;

    indices[0] = 0;
    indices[1] = 1;

    Shape *shape = malloc(sizeof(Shape));
    shape_create(shape, vertices, 2, indices, 2);
    return shape;
}

Shape* shape_create_box(Vec3 min, Vec3 max, Vec3 color)
{
    ShapeVertexData vertices[8];
    u32 indices[24];

    ShapeVertexData v0 = {vec3_create(min.x, max.y, min.z), color};
    ShapeVertexData v1 = {vec3_create(min.x, max.y, max.z), color};
    ShapeVertexData v2 = {vec3_create(min.x, min.y, max.z), color};
    ShapeVertexData v3 = {vec3_create(min.x, min.y, min.z), color};
    ShapeVertexData v4 = {vec3_create(max.x, max.y, min.z), color};
    ShapeVertexData v5 = {vec3_create(max.x, max.y, max.z), color};
    ShapeVertexData v6 = {vec3_create(max.x, min.y, max.z), color};
    ShapeVertexData v7 = {vec3_create(max.x, min.y, min.z), color};

    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
    vertices[4] = v4;
    vertices[5] = v5;
    vertices[6] = v6;
    vertices[7] = v7;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 1;
    indices[3] = 2;
    indices[4] = 2;
    indices[5] = 3;
    indices[6] = 3;
    indices[7] = 0;

    indices[8] = 4;
    indices[9] = 5;
    indices[10] = 5;
    indices[11] = 6;
    indices[12] = 6;
    indices[13] = 7;
    indices[14] = 7;
    indices[15] = 4;

    indices[16] = 0;
    indices[17] = 4;
    indices[18] = 1;
    indices[19] = 5;
    indices[20] = 2;
    indices[21] = 6;
    indices[22] = 3;
    indices[23] = 7;

    Shape *shape = malloc(sizeof(Shape));
    shape_create(shape, vertices, 8, indices, 24);
    return shape;
}

Shape* shape_create_circle(Vec3 center, float radius, Color color, u32 segments, float start_angle, float end_angle)
{
    //TODO: buffer overflow somewhere here
    Vec3 *points = malloc(sizeof(Vec3) * segments);
    float step = 0;
    if (end_angle > start_angle)
    {
        step = (end_angle - start_angle) / segments;
    }
    else
    {
        step = ((360 - start_angle) + end_angle) / segments;
    }

    for (u32 i = 0; i < segments + 1; ++i)
    {
        float angle = start_angle + i * step;
        float x;
        float y;
        math_get_circle_coord(radius, center.x, center.y, angle, &x, &y);
        points[i] = vec3_create(x, y, 0);
    }

    ShapeVertexData* vertices = malloc(sizeof(ShapeVertexData) * segments);
    u32 *indices = malloc(sizeof(u32) * segments * 2);

    Vec3 color_vec = color_to_vec3(color);
    u32 index_ctr = 0;

    for (u32 i = 0; i < segments; ++i) {
        ShapeVertexData v0 = {points[i], color_vec};
        vertices[i] = v0;
        indices[index_ctr++] = i;
        indices[index_ctr++] = i + 1;
    }

    Shape *shape = malloc(sizeof(Shape));
    shape_create(shape, vertices, segments, indices, segments * 2);

    free(points);
    free(vertices);
    free(indices);

    return shape;
}

Shape* shape_create_bone(float width, float height, Vec3 color)
{
    ShapeVertexData vertices[6];
    u32 indices[24];

    float w = width / 2;
    float h = height;
    float hh = height / 2;

    ShapeVertexData v0 = {vec3_create(0, h, 0), color};
    ShapeVertexData v1 = {vec3_create(-w, hh, -w), color};
    ShapeVertexData v2 = {vec3_create(-w, hh, w), color};
    ShapeVertexData v3 = {vec3_create(w, hh, w), color};
    ShapeVertexData v4 = {vec3_create(w, hh, -w), color};
    ShapeVertexData v5 = {vec3_create(0, 0, 0), color};

    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
    vertices[4] = v4;
    vertices[5] = v5;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 0;
    indices[3] = 2;
    indices[4] = 0;
    indices[5] = 3;
    indices[6] = 0;
    indices[7] = 4;

    indices[8] = 5;
    indices[9] = 1;
    indices[10] = 5;
    indices[11] = 2;
    indices[12] = 5;
    indices[13] = 3;
    indices[14] = 5;
    indices[15] = 4;

    indices[16] = 1;
    indices[17] = 2;
    indices[18] = 2;
    indices[19] = 3;
    indices[20] = 3;
    indices[21] = 4;
    indices[22] = 4;
    indices[23] = 1;

    Shape *shape = malloc(sizeof(Shape));
    shape_create(shape, vertices, 6, indices, 24);
    return shape;
}

Shape* shape_create_grid(float width, float height, u32 segs_x, u32 segs_y, Vec3 color)
{
    ShapeVertexData* vertices = malloc(sizeof(ShapeVertexData) * segs_x * 2 * segs_y * 2);
    u32 *indices = malloc(sizeof(u32) * segs_x * 2 * segs_y * 2);

    float x_step = width / segs_x;
    float y_step = height / segs_y;

    u32 vertex_ctr = 0;
    u32 index_ctr = 0;
    for (u32 i = 0; i <= segs_x; ++i) {
        float pos = i * x_step;

        ShapeVertexData v0 = {vec3_create(pos, 0, 0), color};
        ShapeVertexData v1 = {vec3_create(pos, 0, height), color};

        vertices[vertex_ctr++] = v0;
        vertices[vertex_ctr++] = v1;
    }

    for (u32 i = 0; i <= segs_y; ++i) {
        float pos = i * y_step;

        ShapeVertexData v0 = {vec3_create(0, 0, pos), color};
        ShapeVertexData v1 = {vec3_create(width, 0, pos), color};

        vertices[vertex_ctr++] = v0;
        vertices[vertex_ctr++] = v1;
    }

    for (u32 i = 0; i < segs_x * 2 * segs_y * 2; ++i) {
        indices[index_ctr++] = i;
    }

    Shape *shape = malloc(sizeof(Shape));
    shape_create(shape, vertices, vertex_ctr, indices, index_ctr);

    free(vertices);
    free(indices);

    return shape;
}

Shape* shape_create_normals_for_mesh(Mesh* mesh, Vec3 color, float length)
{
    /*
    u32 num_normals = mesh->num_vertices;

    std::vector<ShapeVertexData> vertices;
    std::vector<u32> indices;

    u32 index_ctr = 0;
    for (u32 i = 0; i < num_normals; ++i)
    {
        Vec3 p0 = mesh->vertices[i].position;
        Vec3 p1 = p0 + mesh->vertices[i].normal * length;

        ShapeVertexData vert0;
        vert0.position = p0;
        vert0.color = color;

        ShapeVertexData vert1;
        vert1.position = p1;
        vert1.color = color;

        vertices.push_back(vert0);
        vertices.push_back(vert1);

        indices.push_back(index_ctr++);
        indices.push_back(index_ctr++);
    }

    Shape *shape = new Shape();
    shape->create(&vertices[0], vertices.size(), &indices[0], indices.size());

    return shape;
     */

    return NULL;
}
