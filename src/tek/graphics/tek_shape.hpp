#ifndef _TEK_SHAPE_HPP
#define _TEK_SHAPE_HPP

#include "../math/tek_math.hpp"
#include "tek_rect.hpp"
#include "tek_color.hpp"
#include "tek_mesh.hpp"

typedef struct shape_vertex_data_S
{
    Vec3 position;
    Vec3 color;        
}ShapeVertexData;

ShapeVertexData shape_vertex_data_create(Vec3 position, Vec3 color);

typedef struct shape_S
{
    u32 vao;
    u32 vbo;
    u32 ibo;
    u32 num_vertices;
    u32 num_indices;    
}Shape;

void shape_destroy(Shape* shape);

void shape_render(Shape* shape);

void shape_create(Shape* shape, ShapeVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices);

Shape* shape_create_line(Vec3 p0, Vec3 p1, Vec3 color);
Shape* shape_create_box(Vec3 min, Vec3 max, Vec3 color);
Shape* shape_create_circle(Vec3 center, float radius, Color color, u32 segments, float start_angle, float end_angle);
Shape* shape_create_bone(float width, float height, Vec3 color);
Shape* shape_create_grid(float width, float height, u32 segs_x, u32 segs_y, Vec3 color);
Shape* shape_create_normals_for_mesh(Mesh* mesh, Vec3 color, float length);

#endif