#ifndef _TEK_SHADER_HPP
#define _TEK_SHADER_HPP

#include "../utils/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "tek_opengl.hpp"

typedef struct shader_s
{
    u32 program;
    char name[256];
}Shader;

void shader_destroy(Shader* shader);

void shader_bind(Shader* shader);
void shader_uniform_int(Shader* shader, const char* name, int val);
void shader_uniform_int_array(Shader* shader, const char* name, int* val, int count);
void shader_uniform_float(Shader* shader, const char* name, float val);
void shader_uniform_float_array(Shader* shader, const char* name, float* val, int count);
void shader_uniform_vec2(Shader* shader, const char* name, Vec2 val);
void shader_uniform_vec3(Shader* shader, const char* name, Vec3 val);
void shader_uniform_vec4(Shader* shader, const char* name, Vec4 val);
void shader_uniform_mat4(Shader* shader, const char* name, Mat4* val, u32 count, bool transpose);

Shader* shader_load(const char* filename);


#endif
