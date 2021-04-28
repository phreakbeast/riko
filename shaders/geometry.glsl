#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec4 a_color;

uniform mat4 u_mvp;
uniform mat4 u_world;
uniform mat4 u_light_mvp;

out VertexData
{
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec4 position;
	vec3 world_position;
	vec4 light_space_pos;
}vertex_data;

void main()
{
	gl_Position = u_mvp * vec4(a_position, 1.0);
	
	vertex_data.position = vec4(a_position,1.0);
	vertex_data.uv = a_uv;
	vertex_data.color = a_color;
	vertex_data.normal = (u_world * vec4(a_normal, 0.0)).xyz;
	vertex_data.world_position = (u_world * vec4(a_position,1.0)).xyz;
	vertex_data.light_space_pos = u_light_mvp * vec4(a_position, 1.0);
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
	vec4 light_space_pos;
}vertex_data;

#in lighting.glsl
#in material.glsl

uniform DirectionalLight u_direct_light;

uniform vec3 u_eye_world_pos;
uniform int u_use_light;

uniform Material u_material;

uniform int u_num_point_lights;
uniform PointLight u_point_lights[8];

layout(binding=3) uniform sampler2D u_shadow_map;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 light_color = vec4(1,1,1,1);	

	vec4 diffuse_color = material_get_diffuse_color(u_material, vertex_data.uv);
	vec4 specular_color = material_get_specular_color(u_material, vertex_data.uv);

	if(u_use_light == 1)
	{
		light_color = calculate_light(u_direct_light, vertex_data.normal, u_direct_light.direction, 
			u_eye_world_pos, vertex_data.world_position, u_material.specular_level, u_material.specular_power, 1.0);
	}	

	for(int i=0; i<u_num_point_lights; i++)
	{
		light_color += calculate_point_light(u_point_lights[i], vertex_data.normal, vertex_data.world_position, 
			u_eye_world_pos, u_material.specular_power, u_material.specular_level, vertex_data.light_space_pos, u_shadow_map);
	}

	out_color = diffuse_color * specular_color * light_color;
}