#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <cglm/cglm.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct Shader
{
	unsigned int ID;
	unsigned int vertex;
	unsigned int fragment;
}Shader;

void ShaderConstructor(Shader *shader_program, const char* vertexPath, const char* fragmentPath);
void useShader(Shader *shader_program);

void ShaderSetBool(Shader *shader_program, const char *name, bool value);
void ShaderSetInt(Shader *shader_program, const char *name, int value);
void ShaderSetFloat(Shader *shader_program, const char *name, float value);

void ShaderSetVec2(Shader *shader_program, const char *name, vec2 value);
void ShaderSetVec3(Shader *shader_program, const char *name, vec3 value);
void ShaderSetVec4(Shader *shader_program, const char *name, vec4 value);

void ShaderSetMat2(Shader *shader_program, const char *name, mat2 mat);
void ShaderSetMat3(Shader *shader_program, const char *name, mat3 mat);
void ShaderSetMat4(Shader *shader_program, const char *name, mat4 mat);



#endif
