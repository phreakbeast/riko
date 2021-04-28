#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

uniform mat4 u_mvp;

out VertexData
{
	vec4 position;
	vec3 color;
}vertex_data;

void main()
{
	vertex_data.position = vec4(a_position,1.0);
	gl_Position = u_mvp * vec4(a_position,1.0);
	vertex_data.color = a_color;
}

#fs

#version 430 core

in VertexData
{
	vec4 position;
	vec3 color;
}vertex_data;

layout(location = 0) out vec3 out_color;

void main()
{		
	out_color = vertex_data.color;
}