#include "tek_light.h"

#include "tek_opengl.h"
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
	ShadowMapFBO* shadow_map = malloc(sizeof(ShadowMapFBO));
	assert(shadow_map);

	glGenFramebuffers(1, &shadow_map->fbo);

	glGenTextures(1, &shadow_map->shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map->shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map->fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map->shadow_map, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error, status: 0x%x\n", Status);
		free(shadow_map);
		return NULL;
	}

	return shadow_map;
}

void shadow_map_destroy(ShadowMapFBO* shadow_map)
{
	glDeleteFramebuffers(1, &shadow_map->fbo);
	glDeleteTextures(1, &shadow_map->shadow_map);
}

void shadow_map_bind_for_writing(ShadowMapFBO* shadow_map)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_map->fbo);
}

void shadow_map_bind_for_reading(ShadowMapFBO* shadow_map, u32 texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, shadow_map->shadow_map);
}

void shadow_map_unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}