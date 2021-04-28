#include "tek_light.hpp"

#include "tek_opengl.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Mat4 shadow_matrix(Vec3 position, Vec3 rotation, Mat4 projection)
{
	Mat4 trans = mat4_translate(-position.x, -position.y, -position.z);
	Mat4 rot_x = mat4_rotate(rotation.x, 1, 0, 0);
	Mat4 rot_y = mat4_rotate(-rotation.y, 0, 1, 0);
	Mat4 rot = mat4_mul2(&rot_x, &rot_y);

	Mat4 shadow = mat4_mul2(&rot, &trans);	

	return mat4_mul2(&projection,&shadow);
}

ShadowMapFBO* shadow_map_create(u32 window_width, u32 window_height)
{
	ShadowMapFBO* shadow_map = (ShadowMapFBO*)malloc(sizeof(ShadowMapFBO));
	assert(shadow_map);

	shadow_map->width = window_width;
	shadow_map->height = window_height;
	glGenFramebuffers(1, &shadow_map->fbo);

	GLCall(glGenTextures(1, &shadow_map->shadow_map));
	GLCall(glBindTexture(GL_TEXTURE_2D, shadow_map->shadow_map));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, shadow_map->fbo));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map->shadow_map, 0));

	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	GLCall(GLenum status_res = glCheckFramebufferStatus(GL_FRAMEBUFFER));

	if (status_res != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error, status: 0x%x\n", status_res);
		free(shadow_map);
		return NULL;
	}

	return shadow_map;
}

void shadow_map_destroy(ShadowMapFBO* shadow_map)
{
	GLCall(glDeleteFramebuffers(1, &shadow_map->fbo));
	GLCall(glDeleteTextures(1, &shadow_map->shadow_map));
}

void shadow_map_bind_for_writing(ShadowMapFBO* shadow_map)
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_map->fbo));
}

void shadow_map_bind_for_reading(ShadowMapFBO* shadow_map, u32 texture_unit)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, shadow_map->shadow_map));
}

void shadow_map_unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
