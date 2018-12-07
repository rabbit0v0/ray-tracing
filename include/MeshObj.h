#ifndef __MeshObj_h_
#define __MeshObj_h_
#include "glad/glad.h"
#include "tiny_obj_loader.h"

typedef struct MeshObj_
{
	GLuint buffer;
	GLuint vao;
	GLuint ubo;
	GLuint uniform_index;
	GLuint normal_offset;
	GLuint texcoord_offset;
} MeshObj;

int meshCreate(tinyobj::attrib_t *attrib, std::vector<tinyobj::shape_t> *shapes,
			   std::vector<tinyobj::material_t> *materials, MeshObj *mesh_obj, GLuint program);

void meshDrawSelf(MeshObj &mesh_obj);

#endif
