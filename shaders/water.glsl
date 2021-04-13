#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

layout(location = 0) uniform mat4 u_mvp;

out VertexData
{
    vec2 uv;
    vec3 normal;
    vec4 position;
}vertex_data;

void main()
{
    gl_Position = u_mvp * vec4(a_position, 1.0);

    vertex_data.position = vec4(a_position,1.0);
    vertex_data.uv = a_uv;
    vertex_data.normal = (vec4(a_normal, 0.0)).xyz;
}

#fs

#version 430 core

in VertexData
{
    vec2 uv;
    vec3 normal;
    vec4 position;
}vertex_data;

layout(location = 0) out vec4 out_color;

layout(location = 1) uniform float u_time;

void main()
{
    out_color = vec4(0.0,0.0,1.0,0.5);
}