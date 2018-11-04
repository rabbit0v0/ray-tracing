#ifndef __ShaderLoader_h_
#define __ShaderLoader_h_
#include "glad/glad.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FILENAME_LEN 256

/*
 * const char *shader : shader's name, end with { .vert, .tesc, .tese, .geom, .frag, .comp }
 */
void loadShader(const char *shader);

void useProgram();

#ifdef __cplusplus
}
#endif
#endif
