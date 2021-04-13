#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;


layout(location = 0) uniform mat4 u_mvp;

out VertexData
{
	vec4 position;
	vec3 color;
}vertex_data;

void main()
{
	vertex_data.position = u_mvp * vec4(a_position,1.0);
	gl_Position = vertex_data.position;
	vertex_data.color = a_color;
}

#fs

#version 430 core

in VertexData
{
	vec4 position;
	vec3 color;
}vertex_data;

layout(location = 0) out vec4 out_color;

void main()
{		
	out_color = vec4(vertex_data.color, 1.0);
}