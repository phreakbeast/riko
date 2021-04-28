#vs

#version 430 core

layout(location=0) in vec4 a_position;
layout(location=1) in vec2 a_uv;
layout(location=2) in vec4 a_color;

uniform mat4 u_mvp;

out VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data;

void main()
{
	vertex_data.position = u_mvp * a_position;
	gl_Position = vertex_data.position;
	vertex_data.uv = a_uv;
	vertex_data.color = a_color;
}

#fs

#version 430 core

in VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data;

uniform sampler2D u_texture;
uniform int u_use_texture;

layout (location = 0) out vec4 out_color;

void main()
{
	vec4 tex_color = vertex_data.color;
	if(u_use_texture == 1)
	{
		tex_color = texture2D(u_texture, vertex_data.uv.xy);
	}
	
	//out_color = tex_color;

	float depth = texture(u_texture, vertex_data.uv).x;
	depth = 1.0 - (1.0 - depth) * 25.0;
	out_color = vec4(depth,depth,depth,1.0);
}