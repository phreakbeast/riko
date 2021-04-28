#include "tek_shader.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static u32 cur_program = 0;

static int get_uniform_loc(Shader* shader, const char* name)
{
    int loc = -1;
    GLCall(loc = glGetUniformLocation(shader->program, name));
    if(loc < 0)
    {
#ifdef TEK_DEBUG
        printf("Warning: cannot find uniform %s. In shader %s\n", name, shader->name);
#endif
    }
    return loc;
}

void shader_destroy(Shader* shader)
{
    GLCall(glDeleteProgram(shader->program));
}

void shader_bind(Shader* shader)
{
    if(cur_program != shader->program)
    {
        GLCall(glUseProgram(shader->program));
        cur_program = shader->program;
    }
}

void shader_uniform_int(Shader* shader, const char* name, int val)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform1i(loc, val));
}

void shader_uniform_int_array(Shader* shader, const char* name, int* val, int count)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform1iv(loc, count, val));
}

void shader_uniform_float(Shader* shader, const char* name, float val)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform1f(loc, val));
}

void shader_uniform_float_array(Shader* shader, const char* name, float* val, int count)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform1fv(loc, count, val));
}

void shader_uniform_vec2(Shader* shader, const char* name, Vec2 val)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform2f(loc, val.x, val.y));
}

void shader_uniform_vec3(Shader* shader, const char* name, Vec3 val)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform3f(loc, val.x, val.y, val.z));
}

void shader_uniform_vec4(Shader* shader, const char* name, Vec4 val)
{
    int loc = get_uniform_loc(shader, name);
    GLCall(glUniform4f(loc, val.x, val.y, val.z, val.z));
}

void shader_uniform_mat4(Shader* shader, const char* name, Mat4* val, u32 count, bool transpose)
{
    int loc = get_uniform_loc(shader, name);
    if(transpose)
    {
        GLCall(glUniformMatrix4fv(loc, count, GL_TRUE, &val->m[0]));
    }
    else
    {
        GLCall(glUniformMatrix4fv(loc, count, GL_FALSE, &val->m[0]));
    }
    
}

static GLuint create_shader(const char* src, GLenum type)
{
    switch(type)
    {
        case GL_VERTEX_SHADER: printf("compiling vertex shader ...\n");
            break;
        case GL_FRAGMENT_SHADER: printf("compiling fragment shader ...\n");
            break;
        case GL_GEOMETRY_SHADER: printf("compiling geometry shader ...\n");
            break;
        default:
        {
            printf("ERROR: Invalid Shader type\n");
            return 0;
        }
    }
    
    GLCall(GLuint id = glCreateShader(type));
    
    const char* c_src = src;
    GLCall(glShaderSource(id, 1, &c_src, NULL));
    GLCall(glCompileShader(id));
    
    GLCall(GLint result = GL_FALSE);
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(!result)
    {
        int info_length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
        char* error_message = (char*)malloc(sizeof(char) * info_length + 1);
        GLCall(glGetShaderInfoLog(id, info_length, NULL, &error_message[0]));
        printf("ERROR: Shader compilation error: %s \n", error_message);
        GLCall(glDeleteShader(id));
        free(error_message);
        return 0;
    }
    
    return id;
}

static GLuint link_shader(GLuint vert_id, GLuint frag_id, GLuint geom_id)
{
    printf("linking shader ...\n");
    GLCall(GLuint program = glCreateProgram());
    
    GLCall(glAttachShader(program, vert_id));
    GLCall(glAttachShader(program, frag_id));
    if(geom_id > 0)
    {
        GLCall(glAttachShader(program, geom_id));
    }
    
    GLCall(glLinkProgram(program));
    GLint result = GL_FALSE;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if(!result)
    {
        int info_length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length));
        char* error_message = (char*)malloc(sizeof(char) * info_length + 1);
        GLCall(glGetProgramInfoLog(program, info_length, NULL, &error_message[0]));
        printf("ERROR: Shader linking error: %s\n", error_message);
        GLCall(glDeleteProgram(program));
        free(error_message);
        return 0;
    }
    
    return program;
}

static char* include_source(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }
    
    fseek(fp, 0L, SEEK_END);
    u64 buffsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    char* line = (char*)malloc(sizeof(char) * buffsize + 1);
    assert(line);
    size_t new_len = fread(line, sizeof(char), buffsize, fp);
    
    line[new_len++] = '\0';
    
    fclose(fp);
    
    return line;
}

typedef enum source_type_e
{
    SOURCE_VERTEX,
    SOURCE_FRAGMENT,
    SOURCE_GEOMETRY,
    NUM_SOURCE_TYPES
}SourceType;

typedef struct shader_source_s
{
    char* line;
    u32 lenght;
}ShaderSource;

void ss_init(ShaderSource* ss)
{
    ss->line = NULL;
    ss->lenght = 0;
}

void ss_destroy(ShaderSource* ss)
{
    free(ss->line);
}

void ss_add(ShaderSource* ss, const char* text)
{
    size_t len = strlen(text);
    if (ss->lenght == 0)
    {        
        ss->line = (char*)malloc(len + 1);
        assert(ss->line);
        memcpy(ss->line, text, len);
        ss->lenght = len;
        ss->line[ss->lenght] = '\0';
    }
    else
    {        
        size_t new_len = ss->lenght + len;
        ss->line = (char*)realloc(ss->line, new_len + 1);
        assert(ss->line);
        memcpy(&ss->line[ss->lenght], text, len);
        ss->lenght = new_len;
        ss->line[ss->lenght] = '\0';
    }
}

static bool load_source(const char* filename, ShaderSource* vert_src, ShaderSource* frag_src, ShaderSource* geom_src)
{    
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return false;
    }
    
    SourceType source_type = SOURCE_VERTEX;
    
    char line[1024];
    while(fgets(line, 1024, fp))
    {
        if(line[0] == '#' && line[1] == 'i' && line[2] == 'n')
        {
            char file[64];
            int res = sscanf(line, "#in %s", file);
            if(res != 1)
            {
                printf("Warning: cannot process line: %s File: %s\n", line, filename);
                fclose(fp);
                return 0;
            }
            
            char path[128];
            sprintf(path, "%s/%s", "shaders", file);
            
            char* inc = include_source(path);
            if (source_type == SOURCE_VERTEX)
            {
                ss_add(vert_src, inc);
            }
            else if (source_type == SOURCE_FRAGMENT)
            {
                ss_add(frag_src, inc);
            }
            else if (source_type == SOURCE_GEOMETRY)
            {
                ss_add(geom_src, inc);
            }            
            free(inc);
        }
        else if(line[0] == '#' && line[1] == 'g' && line[2] == 's')
        {
            source_type = SOURCE_GEOMETRY;
        }
        else if(line[0] == '#' && line[1] == 'v' && line[2] == 's')
        {
            source_type = SOURCE_VERTEX;
        }
        else if(line[0] == '#' && line[1] == 'f' && line[2] == 's')
        {
            source_type = SOURCE_FRAGMENT;
        }
        else
        {
            if (source_type == SOURCE_VERTEX)
            {
                ss_add(vert_src, line);
            }
            else if (source_type == SOURCE_FRAGMENT)
            {
                ss_add(frag_src, line);
            }
            else if (source_type == SOURCE_GEOMETRY)
            {
                ss_add(geom_src, line);
            }            
        }
    }
    fclose(fp);
    
    return true;
}


Shader* shader_load(const char* filename)
{
    printf("loading shader %s\n", filename);
    
    char path[128];
    sprintf(path, "%s%s", "shaders/", filename);
    //printf("shader path %s\n", path);
    
    ShaderSource vert_src;
    ShaderSource frag_src;
    ShaderSource geom_src;
    ss_init(&vert_src);
    ss_init(&frag_src);
    ss_init(&geom_src);
    if(!load_source(path, &vert_src, &frag_src, &geom_src))
    {
        return NULL;
    }
    
    
    u32 vert_id = create_shader(vert_src.line, GL_VERTEX_SHADER);
    assert(vert_id != 0);
    u32 frag_id = create_shader(frag_src.line, GL_FRAGMENT_SHADER);
    //printf("%s\n", frag_src.c_str());
    assert(frag_id != 0);
    
    u32 geom_id = 0;
    if(geom_src.lenght > 0)
    {
        geom_id = create_shader(geom_src.line, GL_GEOMETRY_SHADER);
        assert(geom_id != 0);
    }
    
    ss_destroy(&vert_src);
    ss_destroy(&frag_src);
    ss_destroy(&geom_src);

    Shader* shader = (Shader*)malloc(sizeof(Shader));
    assert(shader);
    strcpy(shader->name,filename);
    shader->program = link_shader(vert_id, frag_id, geom_id);
    assert(shader->program > 0);

    return shader;
}