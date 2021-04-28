
struct Material
{
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;

	int has_diffuse_map;
	int has_specular_map;
	int has_normal_map;

		sampler2D diffuse_map;
		sampler2D specular_map;
		sampler2D normal_map;

	float specular_level;
	float specular_power;
};

vec4 material_get_diffuse_color(Material mat, vec2 uv)
{
	vec4 res_color;
	if(mat.has_diffuse_map == 1)
	{
		res_color = texture2D(mat.diffuse_map, uv.xy);
	}
	else
	{
		res_color = mat.diffuse_color + mat.ambient_color;
	}
	return res_color; 
}

vec4 material_get_specular_color(Material mat, vec2 uv)
{
	vec4 res_color;
	
	if(mat.has_specular_map == 1)
	{
		res_color = texture2D(mat.specular_map, uv.xy);
	}
	else
	{
		res_color = mat.specular_color;
	}
	
	return res_color; 
}

vec4 material_get_normal_color(Material mat, vec2 uv)
{
	vec4 res_color = texture2D(mat.normal_map, uv.xy);
	
	return res_color; 
}