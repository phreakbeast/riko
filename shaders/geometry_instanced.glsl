#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec4 a_color;
layout(location = 4) in mat4 a_mvp;
layout(location = 8) in mat4 a_world;

out VertexData
{
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec4 position;
	vec3 world_position;
}vertex_data;

void main()
{
	gl_Position = a_mvp * vec4(a_position, 1.0);
	
	vertex_data.position = vec4(a_position,1.0);
	vertex_data.uv = a_uv;
	vertex_data.color = a_color;
	vertex_data.normal = (a_world * vec4(a_normal, 0.0)).xyz;
	vertex_data.world_position = (a_world * vec4(a_position,1.0)).xyz;
}


#fs

#version 430 core

in VertexData
{
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec4 position;
	vec3 world_position;
}vertex_data;

#in lighting.glsl

uniform DirectionalLight u_direct_light;


uniform vec3 u_eye_world_pos;
uniform int u_use_light;

uniform sampler2D u_texture;
uniform int u_use_texture;

layout(location = 0) out vec4 out_color;

void main()
{
	//light color
	vec4 light_color = vec4(1,1,1,1);
	vec4 tex_color = vertex_data.color;
	if(u_use_light == 1)
	{
		light_color = calculate_light(u_direct_light, vertex_data.normal, u_direct_light.direction, u_eye_world_pos, vertex_data.world_position, 0, 0, 1);
	}

	if(u_use_texture == 1)
	{
		tex_color = texture(u_texture,vertex_data.uv);
	}

	//out_color = vertex_data.color * light_color;
	out_color = tex_color;
}