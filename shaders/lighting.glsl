
struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    vec3 position;
    Attenuation atten;
};

float calc_shadow_factor(vec4 light_space_pos, in sampler2D shadow_map)
{
    vec3 proj_coords = light_space_pos.xyz / light_space_pos.w;
    vec2 uv_coords;
    uv_coords.x = 0.5 * proj_coords.x + 0.5;
    uv_coords.y = 0.5 * proj_coords.y + 0.5;
    float z = 0.5 * proj_coords.z + 0.5;
    float depth = texture(shadow_map, uv_coords).x;
    if (depth < (z + 0.00001))
        return 0.5;
    else
        return 1.0;
}

vec4 calculate_light(DirectionalLight light, vec3 normal, vec3 direction, vec3 eye_world_pos, vec3 world_pos, float specular_intensity, float specular_power, float shadow_factor)
{
	vec4 ambient_color = vec4(light.color * light.intensity, 1.0);
	float diffuse_factor = dot(normalize(normal), -direction);

	vec4 diffuse_color = vec4(0,0,0,1);
	vec4 specular_color = vec4(0,0,0,1);

	if(diffuse_factor > 0)
	{
		diffuse_color = vec4(light.color, 1.0) * light.intensity * diffuse_factor;

		vec3 vertex_to_eye = normalize(eye_world_pos - world_pos);
		vec3 light_reflect = normalize(reflect(direction,normal));
		float specular_factor = dot(vertex_to_eye, light_reflect);
		if(specular_factor > 0)
		{
			specular_factor = pow(specular_factor, specular_power);
			specular_color = vec4(light.color * specular_intensity * specular_factor, 1.0);
		}
	}	
	
	//return diffuse_color;
	return (ambient_color + shadow_factor * (diffuse_color + specular_color));
}

vec4 calculate_point_light(PointLight light, vec3 normal, vec3 world_pos, vec3 eye_world_pos, float specular_power, float specular_intensity, vec4 light_space_pos, in sampler2D shadow_map)
{
    vec3 light_direction = world_pos - light.position;
    float distance = length(light_direction);
    light_direction = normalize(light_direction);
    float shadow_factor = calc_shadow_factor(light_space_pos, shadow_map);
    
    vec4 ambient_color = vec4(light.color, 1.0f) * light.ambient_intensity;
    float diffuse_factor = dot(normal, -light_direction);

    vec4 diffuse_color = vec4(0, 0, 0, 0);
    vec4 specular_color = vec4(0, 0, 0, 0);

    if (diffuse_factor > 0) 
    {
        diffuse_color = vec4(light.color * light.diffuse_intensity * diffuse_factor, 1.0f);
        vec3 vertex_to_eye = normalize(eye_world_pos - world_pos);
        vec3 light_reflect = normalize(reflect(light_direction, normal));
        float specular_factor = dot(vertex_to_eye, light_reflect);
        if (specular_factor > 0) 
        {
            specular_factor = pow(specular_factor, specular_power);
            specular_color = vec4(light.color * specular_intensity * specular_factor, 1.0f);
        }
    }

    vec4 color = (ambient_color + shadow_factor * (diffuse_color + specular_color));


    float attenuation = light.atten.constant +
                        light.atten.linear * distance +
                        light.atten.exp * distance * distance;

    return color / attenuation;
}