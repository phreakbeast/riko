#include "tek_material.h"

#include <stdlib.h>
#include <assert.h>

Material* material_create()
{
	Material* mat = malloc(sizeof(Material));
	assert(mat);
	mat->diffuse_map = NULL;
	mat->specular_map = NULL;
	mat->normal_map = NULL;

	mat->ambient_color = color_create(100, 100, 100);
	mat->diffuse_color = color_create(255, 255, 255);
	mat->specular_color = color_create(255, 255, 255);

	mat->specular_level = 0.5f;
	mat->specular_power = 16;

	return mat;
}

void material_destroy(Material* mat)
{
	if (mat->diffuse_map)
	{
		texture_destroy(mat->diffuse_map);
		free(mat->diffuse_map);
	}

	if (mat->specular_map)
	{
		texture_destroy(mat->specular_map);
		free(mat->specular_map);
	}

	if (mat->normal_map)
	{
		texture_destroy(mat->normal_map);
		free(mat->normal_map);
	}
}