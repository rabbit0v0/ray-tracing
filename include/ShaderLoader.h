#ifndef __ShaderLoader_h_
#define __ShaderLoader_h_
#include "glad/glad.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FILENAME_LEN 256

enum ShaderType
{
	VERT = 0,
	TESC,
	TESE,
	GEOM,
	FRAG,
	COMP,
	ERR
};

typedef struct ShaderObj_
{
	enum ShaderType type;
	GLuint shader;
} ShaderObj;

/*
 * const char *shader : shader's name, end with { .vert, .tesc, .tese, .geom, .frag, .comp }
 */
ShaderObj loadShader(const char *shader);
void attachProgram(GLuint program, ShaderObj shader_obj);
void useProgram(GLuint program);

#ifdef __cplusplus
}
#endif
#endif
