#vs

#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec4 a_color;

layout(location = 0) uniform mat4 u_mvp;
layout(location = 1) uniform mat4 u_world;

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
	gl_Position = u_mvp * vec4(a_position, 1.0);
	
	vertex_data.position = vec4(a_position,1.0);
	vertex_data.uv = a_uv;
	vertex_data.color = a_color;
	vertex_data.normal = (u_world * vec4(a_normal, 0.0)).xyz;
	vertex_data.world_position = (u_world * vec4(a_position,1.0)).xyz;
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
#in material.glsl

layout(location = 2) uniform DirectionalLight u_direct_light;

layout(location = 6) uniform vec3 u_eye_world_pos;
layout(location = 7) uniform int u_use_light;

layout(location = 8) uniform sampler2D u_texture;
layout(location = 9) uniform int u_use_texture;

layout(location = 10) uniform Material u_material;

layout(location = 21) uniform int u_num_point_lights;
layout(location = 22) uniform PointLight u_point_lights[8];

layout(location = 0) out vec4 out_color;

void main()
{
	//light color
	vec4 light_color = vec4(1,1,1,1);	

	vec4 diffuse_color = material_get_diffuse_color(u_material, vertex_data.uv);
	vec4 specular_color = material_get_specular_color(u_material, vertex_data.uv);

	if(u_use_light == 1)
	{
		light_color = calculate_light(u_direct_light, vertex_data.normal, u_direct_light.direction, 
			u_eye_world_pos, vertex_data.world_position, u_material.specular_level, u_material.specular_power, 1);
	}	

	for(int i=0; i<u_num_point_lights; i++)
	{
		light_color += calculate_point_light(u_point_lights[i], vertex_data.normal, vertex_data.world_position, 
			u_eye_world_pos, u_material.specular_power, u_material.specular_level);
	}

	//out_color = vertex_data.color * light_color;
	//out_color = light_color;
	out_color = diffuse_color * light_color;
}