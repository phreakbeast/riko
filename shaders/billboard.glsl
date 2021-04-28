#vs
#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;

uniform mat4 u_mv;
uniform mat4 u_p;

uniform int u_spherical;

out VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data;

void main()
{
	mat4 model_view = u_mv;

	//first column
	model_view[0][0] = 1.0;
	model_view[0][1] = 0.0;
	model_view[0][2] = 0.0;

	if(u_spherical == 1)
	{
		//second column
		model_view[1][0] = 0.0;
		model_view[1][1] = 1.0;
		model_view[1][2] = 0.0;
	}

	//third column
	model_view[2][0] = 0.0;
	model_view[2][1] = 0.0;
	model_view[2][2] = 1.0;

	vec4 p = model_view * vec4(a_position, 1.0);

	vertex_data.position = u_p * p;
	gl_Position = vertex_data.position;
	vertex_data.uv = a_uv;
	vertex_data.color = a_color;
}

#fs
#version 430 core

uniform sampler2D u_texture;

in VertexData
{
	vec4 position;
	vec2 uv;
	vec4 color;
}vertex_data;

layout (location = 0) out vec4 out_color;

void main()
{
	vec4 tex_color = texture2D(u_texture, vertex_data.uv);
	out_color = tex_color * vertex_data.color;
}