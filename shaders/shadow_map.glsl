#vs

#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_mvp;

out VertexData
{
	vec2 uv;
}vertex_data;

void main()
{
    gl_Position = u_mvp * vec4(a_position, 1.0);
    vertex_data.uv = a_uv;
}

#fs

#version 430 core

in VertexData
{
	vec2 uv;
}vertex_data;

uniform sampler2D u_shadow_map;

layout(location = 0) out vec4 out_color;

void main()
{
    float depth = texture(u_shadow_map, vertex_data.uv).x;
    depth = 1.0 - (1.0 - depth) * 25.0;
    out_color = vec4(depth);
    //out_color = vec4(0,1,0,1);
}