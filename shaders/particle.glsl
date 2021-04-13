#vs
#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec4 a_uv_rect;
layout (location = 3) in float a_size;
layout (location = 4) in float a_life;

layout (location = 0) uniform mat4 u_mv;

out VertexData
{
	vec4 position;
	vec4 color;
	vec4 uv_rect;
	float size;
	float life;
}vertex_data;

void main()
{
	vertex_data.position = u_mv * vec4(a_position, 1.0);
	gl_Position = vertex_data.position;
	vertex_data.color = a_color;
	vertex_data.uv_rect = a_uv_rect;
	vertex_data.size = a_size;
	vertex_data.life = a_life;
}

#gs
#version 430 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout (location = 1) uniform mat4 u_p;

in VertexData
{
	vec4 position;
	vec4 color;
	vec4 uv_rect;
	float size;
	float life;
}vertex_data_in[];

out VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data_out;

void main()
{
	vec4 p = gl_in[0].gl_Position;
	if(vertex_data_in[0].life > 0)
	{
		//vertex 2 left-top
		vec2 vb = p.xy + vec2(-0.5, 0.5) * vertex_data_in[0].size;
		vertex_data_out.position = u_p * vec4(vb, p.zw);
		gl_Position = vertex_data_out.position;
		vertex_data_out.uv = vec2(vertex_data_in[0].uv_rect.x, vertex_data_in[0].uv_rect.w);
		vertex_data_out.color = vertex_data_in[0].color;
		EmitVertex();

		//vertex 1 left-bottom
		vec2 va = p.xy + vec2(-0.5, -0.5) * vertex_data_in[0].size;
		vertex_data_out.position = u_p * vec4(va, p.zw);
		gl_Position = vertex_data_out.position;
		vertex_data_out.uv = vec2(vertex_data_in[0].uv_rect.x, vertex_data_in[0].uv_rect.y);
		vertex_data_out.color = vertex_data_in[0].color;
		EmitVertex();

		//vertex 4 right-top
		vec2 vc = p.xy + vec2(0.5, 0.5) * vertex_data_in[0].size;
		vertex_data_out.position = u_p * vec4(vc, p.zw);
		gl_Position = vertex_data_out.position;
		vertex_data_out.uv = vec2(vertex_data_in[0].uv_rect.z, vertex_data_in[0].uv_rect.w);
		vertex_data_out.color = vertex_data_in[0].color;
		EmitVertex();

		//vertex 3 right-bottom
		vec2 vd = p.xy + vec2(0.5, -0.5) * vertex_data_in[0].size;
		vertex_data_out.position = u_p * vec4(vd, p.zw);
		gl_Position = vertex_data_out.position;
		vertex_data_out.uv = vec2(vertex_data_in[0].uv_rect.z, vertex_data_in[0].uv_rect.y);
		vertex_data_out.color = vertex_data_in[0].color;
		EmitVertex();


		EndPrimitive();
	}
}

#fs
#version 430 core


layout (location = 2) uniform sampler2D u_texture;

in VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data;

layout (location = 0) out vec4 out_color;

void main()
{	
	vec4 t = texture2D(u_texture, vertex_data.uv.xy);
	out_color = t * vertex_data.color;
	if(out_color.a < 0.1)
		discard;
}