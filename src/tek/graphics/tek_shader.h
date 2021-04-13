#ifndef _TEK_SHADER_H
#define _TEK_SHADER_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"
#include "tek_opengl.h"

typedef struct shader_s
{
    u32 program;        
}Shader;

void shader_destroy(Shader* shader);

void shader_bind(Shader* shader);
void shader_uniform_int(Shader* shader, int loc, int val);
void shader_uniform_int_array(Shader* shader, int loc, int* val, int count);
void shader_uniform_float(Shader* shader, int loc, float val);
void shader_uniform_float_array(Shader* shader, int loc, float* val, int count);
void shader_uniform_vec2(Shader* shader, int loc, Vec2 val);
void shader_uniform_vec3(Shader* shader, int loc, Vec3 val);
void shader_uniform_vec4(Shader* shader, int loc, Vec4 val);
void shader_uniform_mat4(Shader* shader, int loc, Mat4* val, u32 count, bool transpose);

Shader* shader_load(const char* filename);


#endif
