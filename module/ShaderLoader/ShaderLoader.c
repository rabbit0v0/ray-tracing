#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShaderLoader.h"

#define ShaderLoader_DEBUG 1

ShaderObj loadShader(const char *shader)
{
	ShaderObj ret;
	ret.type = ERR;
	char shader_type[5];
	strncpy(shader_type, shader + strlen(shader) - 4, 5);

	char fn[MAX_FILENAME_LEN] = "./shaders/";
	strcat(fn, shader_type);
	strcat(fn, "/");
	strcat(fn, shader);

	FILE *fp = fopen(fn, "r");
	if (!fp)
	{
		perror("loadShader: cannot open file"); // TODO
		return ret;
	}
	fseek(fp, 0, SEEK_END);
	long file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *src = (char *)malloc(file_len + 1);
	fread(src, 1, file_len, fp);
	src[file_len] = '\0';
	fclose(fp);

	if (!strncmp(shader_type, "vert", 4))
	{
		ret.type = VERT;
		ret.shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (!strncmp(shader_type, "tesc", 4))
	{
		ret.type = TESC;
		ret.shader = glCreateShader(GL_TESS_CONTROL_SHADER);
	}
	else if (!strncmp(shader_type, "tese", 4))
	{
		ret.type = TESE;
		ret.shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	}
	else if (!strncmp(shader_type, "geom", 4))
	{
		ret.type = GEOM;
		ret.shader = glCreateShader(GL_GEOMETRY_SHADER);
	}
	else if (!strncmp(shader_type, "frag", 4))
	{
		ret.type = FRAG;
		ret.shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else if (!strncmp(shader_type, "comp", 4))
	{
		ret.type = COMP;
		ret.shader = glCreateShader(GL_COMPUTE_SHADER);
	}
	else
	{
		free(src);
		fprintf(stderr, "fail to load shader: filename extension unmatched\n");
		return ret;
	}
	const char *s = src;
	glShaderSource(ret.shader, 1, &s, NULL);
	glCompileShader(ret.shader);
#if ShaderLoader_DEBUG
	GLint result;
	glGetShaderiv(ret.shader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		GLsizei length;
		glGetShaderiv(ret.shader, GL_INFO_LOG_LENGTH, &length);
		GLchar *info_log = (GLchar *)malloc(length + 1);
		glGetShaderInfoLog(ret.shader, length, NULL, info_log);
		info_log[length] = '\0';
		printf("ShaderLoader: %s: Compile err: %s (length %d)\n", shader, info_log, length);
		free(info_log);
		ret.type = ERR;
	}
#endif
	free(src);
	return ret;
}

void attachProgram(GLuint program, ShaderObj shader_obj)
{
	if (glIsShader(shader_obj.shader))
	{
		glAttachShader(program, shader_obj.shader);
		glDeleteShader(shader_obj.shader);
	}
	else
	{
		fprintf(stderr, "attachProgram: shader_obj is not valid\n");
	}
}

void useProgram(GLuint program)
{
	glLinkProgram(program);
	glUseProgram(program);
	glDeleteProgram(program);
}
