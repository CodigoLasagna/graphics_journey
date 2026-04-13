#include "ShaderManager.h"


void ShaderConstructor(Shader* shader_program, const char* vertexPath, const char* fragmentPath)
{
	char* vertexCode;
	char* fragmentCode;
	const char *vcode;
	const char *fcode;
	int success;
	char infoLog[512];
	size_t length = 0;
	size_t read;

	FILE *fptr = fopen(vertexPath, "r");
	if (!fptr)
	{
		printf("Error al abrir el archivo de vertexShader\n");
		return;
	}
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	rewind(fptr);
	vertexCode = malloc(length + 1);
	read = fread(vertexCode, 1, length, fptr);
	if (read != length)
	{
		printf("Error al leer el archivo de vertexShader\n");
		return;
	}
	vertexCode[length] = '\0';
	fclose(fptr);

	fptr = fopen(fragmentPath, "r");
	if (!fptr)
	{
		printf("Error al abrir el archivo de fragmentShader\n");
		return;
	}
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	rewind(fptr);
	fragmentCode = malloc(length + 1);
	read = fread(fragmentCode, 1, length, fptr);
	if (read != length)
	{
		printf("Error al leer el archivo de fragmentShader\n");
		return;
	}
	fragmentCode[length] = '\0';
	fclose(fptr);

	shader_program->vertex = glCreateShader(GL_VERTEX_SHADER);
	vcode = vertexCode;
	glShaderSource(shader_program->vertex, 1, &vcode, NULL);
	glCompileShader(shader_program->vertex);

	/*print errors if found*/
	glGetShaderiv(shader_program->vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader_program->vertex, 512, NULL, infoLog);
		printf("Error shader vertex compilation failed %s\n", infoLog);
	}

	shader_program->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	fcode = fragmentCode;
	glShaderSource(shader_program->fragment, 1, &fcode, NULL);
	glCompileShader(shader_program->fragment);

	/*print errors if found*/
	glGetShaderiv(shader_program->fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader_program->fragment, 512, NULL, infoLog);
		printf("Error shader fragment compilation failed %s\n", infoLog);
	}

	shader_program->ID = glCreateProgram();
	glAttachShader(shader_program->ID, shader_program->vertex);
	glAttachShader(shader_program->ID, shader_program->fragment);
	glLinkProgram(shader_program->ID);

	/*print errors on linking if any*/
	glGetProgramiv(shader_program->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program->ID, 512, NULL, infoLog);
		printf("Error program shader linking failed %s\n", infoLog);
	}

	glDeleteShader(shader_program->vertex);
	glDeleteShader(shader_program->fragment);

	free(vertexCode);
	free(fragmentCode);
}

void useShader(Shader *shader_program)
{
	glUseProgram(shader_program->ID);
}


void ShaderSetBool(Shader *shader_program, const char *name, bool value)
{
	glUniform1i(glGetUniformLocation(shader_program->ID, name), (int)value);
}

void ShaderSetInt(Shader *shader_program, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(shader_program->ID, name), value);
}

void ShaderSetFloat(Shader *shader_program, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(shader_program->ID, name), value);
}

void ShaderSetVec2(Shader *shader_program, const char *name, vec2 value)
{
	glUniform2fv(glGetUniformLocation(shader_program->ID, name), 1, &value[0]);
}

void ShaderSetVec3(Shader *shader_program, const char *name, vec3 value)
{
	glUniform3fv(glGetUniformLocation(shader_program->ID, name), 1, &value[0]);
}

void ShaderSetVec4(Shader *shader_program, const char *name, vec4 value)
{
	glUniform4fv(glGetUniformLocation(shader_program->ID, name), 1, &value[0]);
}

void ShaderSetMat2(Shader *shader_program, const char *name, mat2 mat)
{
	glUniformMatrix2fv(glGetUniformLocation(shader_program->ID, name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderSetMat3(Shader *shader_program, const char *name, mat3 mat)
{
	glUniformMatrix3fv(glGetUniformLocation(shader_program->ID, name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderSetMat4(Shader *shader_program, const char *name, mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shader_program->ID, name), 1, GL_FALSE, &mat[0][0]);
}
