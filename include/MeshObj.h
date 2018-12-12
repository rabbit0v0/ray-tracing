#ifndef __MeshObj_h_
#define __MeshObj_h_
#include "glad/glad.h"
#include "ShaderProgram.h"

typedef struct MeshObj_
{
	ShaderProgram *shader_program;
	GLuint vao;
	GLuint v_buffer;
	GLuint vn_buffer;
	GLuint vt_buffer;
	GLuint v_num;
} MeshObj;

MeshObj meshCreate(ShaderProgram *shader_program, const char *obj_name, const char *mtl_dir);

void meshDrawSelf(MeshObj &mesh_obj);

#endif
