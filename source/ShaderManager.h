#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

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
void use(Shader *shader_program);

void setBool(Shader *shader_program, const char *name, bool value);
void setInt(Shader *shader_program, const char *name, int value);
void setFloat(Shader *shader_program, const char *name, float value);



#endif
