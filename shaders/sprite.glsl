#vs

#version 430 core

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in float a_tid;
layout (location = 3) in vec4 a_color;

layout (location = 0) uniform mat4 u_mvp;

out VertexData
{
	vec4  position;
	vec2  uv;
	float tid;
	vec4  color;
}vertex_data;

void main()
{
	vertex_data.position = a_position;
	gl_Position = u_mvp * a_position;
	vertex_data.uv = a_uv;
	vertex_data.tid = a_tid;
	vertex_data.color = a_color;
}


#fs

#version 430 core

in VertexData
{
	vec4  position;
	vec2  uv;
	float tid;
	vec4  color;
}vertex_data;

layout (location = 1) uniform sampler2D u_texture_0;
layout (location = 2) uniform sampler2D u_texture_1;
layout (location = 3) uniform sampler2D u_texture_2;
layout (location = 4) uniform sampler2D u_texture_3;
layout (location = 5) uniform sampler2D u_texture_4;
layout (location = 6) uniform sampler2D u_texture_5;
layout (location = 7) uniform sampler2D u_texture_6;
layout (location = 8) uniform sampler2D u_texture_7;

layout (location = 0) out vec4 out_color;

void main()
{
	if(vertex_data.tid > 0.0)
	{
		int tid = int(vertex_data.tid - 0.5);
		vec4 tex_color;
		if(tid == 0)
		{
			tex_color = texture(u_texture_0, vertex_data.uv.xy);
		}
		else if(tid == 1)
		{
			tex_color = texture(u_texture_1, vertex_data.uv.xy);
		}
		else if(tid == 2)
		{
			tex_color = texture(u_texture_2, vertex_data.uv.xy);
		}
		else if(tid == 3)
		{
			tex_color = texture(u_texture_3, vertex_data.uv.xy);
		}
		else if(tid == 4)
		{
			tex_color = texture(u_texture_4, vertex_data.uv.xy);
		}
		else if(tid == 5)
		{
			tex_color = texture(u_texture_5, vertex_data.uv.xy);
		}
		else if(tid == 6)
		{
			tex_color = texture(u_texture_6, vertex_data.uv.xy);
		}
		else if(tid == 7)
		{
			tex_color = texture(u_texture_7, vertex_data.uv.xy);
		}
		out_color = tex_color * vertex_data.color;
	}
	else
	{
		out_color =  vertex_data.color;
	}
}