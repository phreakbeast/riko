#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 4) in mat4 a_mvp;
layout(location = 8) in mat4 a_world;
layout(location = 13) in vec3 a_colors;

out VertexData
{
	vec4 position;
	vec3 color;	
	vec3 world_position;
}vertex_data;

void main()
{
	gl_Position = a_mvp * vec4(a_position, 1.0);

	vertex_data.position = vec4(a_position,1.0);
	vertex_data.color = a_colors;
	vertex_data.world_position = (a_world * vec4(a_position,1.0)).xyz;
}

#fs

#version 430 core

in VertexData
{
	vec4 position;
	vec3 color;	
	vec3 world_position;
}vertex_data;

layout(location = 0) out vec3 out_color;

void main()
{		
	out_color = vertex_data.color;
}