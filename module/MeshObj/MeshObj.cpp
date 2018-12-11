#include "MeshObj.h"
#include "tiny_obj_loader.h"

#include <stdio.h>
#include <cassert>

#define PRINT_LINE printf("%s: %d\n", __FILE__, __LINE__)

enum Attrib_IDs
{
	VertexPosition = 0,
	VertexNormal,
	VertexTexcoord
};

MeshObj meshCreate(ShaderProgram *shader_program, const char *obj_name, const char *mtl_dir)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, obj_name, mtl_dir);
	if (!err.empty())
	{
		printf("tinyobj: %s\n", err.c_str());
	}
	if (!ret)
	{
		exit(1);
	}

	MeshObj mesh_obj;
	mesh_obj.shader_program = shader_program;

	glGenBuffers(1, &(mesh_obj.buffer));
	glGenVertexArrays(1, &(mesh_obj.vao));
	glBindBuffer(GL_ARRAY_BUFFER, mesh_obj.buffer);
	glBindVertexArray(mesh_obj.vao);

	GLfloat *temp_v_buffer = (GLfloat *)malloc(200*attrib.vertices.size() * sizeof(tinyobj::real_t));
	GLfloat *temp_n_buffer = (GLfloat *)malloc(200*attrib.normals.size() * sizeof(tinyobj::real_t));
	GLfloat *temp_t_buffer = (GLfloat *)malloc(200*attrib.texcoords.size() * sizeof(tinyobj::real_t));

	int v_offset = 0;
	int n_offset = 0;
	int t_offset = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];
			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				temp_v_buffer[v_offset] = vx;
				temp_v_buffer[v_offset + 1] = vy;
				temp_v_buffer[v_offset + 2] = vz;
				v_offset += 3;

				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

				temp_n_buffer[n_offset] = nx;
				temp_n_buffer[n_offset + 1] = ny;
				temp_n_buffer[n_offset + 2] = nz;
				n_offset += 3;

//				tinyobj::real_t tx = attrib->texcoords[2 * idx.texcoord_index + 0];
//				tinyobj::real_t ty = attrib->texcoords[2 * idx.texcoord_index + 1];

//				temp_t_buffer[t_offset] = tx;
//				temp_t_buffer[t_offset + 1] = ty;
				t_offset += 2;
			}
			index_offset += fv;
			//			shapes[s].mesh.material_ids[f];
		}
	}

	glBufferData(GL_ARRAY_BUFFER, (v_offset + n_offset + t_offset) * sizeof(tinyobj::real_t), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v_offset * sizeof(tinyobj::real_t), temp_v_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, v_offset * sizeof(tinyobj::real_t), n_offset * sizeof(tinyobj::real_t),
					temp_n_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, (v_offset + n_offset) * sizeof(tinyobj::real_t),
					t_offset * sizeof(tinyobj::real_t), temp_t_buffer);

	mesh_obj.normal_offset = v_offset;
	mesh_obj.texcoord_offset = v_offset + n_offset;
	free(temp_v_buffer);
	free(temp_n_buffer);
	free(temp_t_buffer);
	glVertexAttribPointer(VertexPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VertexPosition);
	glVertexAttribPointer(VertexNormal, 3, GL_FLOAT, GL_FALSE, 0,
						  (const GLvoid *)((long)mesh_obj.normal_offset * sizeof(tinyobj::real_t)));
	glEnableVertexAttribArray(VertexNormal);
	glVertexAttrib2f(VertexTexcoord, 0.0f, 0.0f);

	const char *uniforms_uniform_var_names[] = {
		"ambient",
		"diffuse",
		"specular",
		"shininess",
		"strength"
	};
	GLfloat ambient[] = {0.3f, 0.3f, 0.3f};
	GLfloat diffuse = 0.3f;
	GLfloat specular = 0.3f;
	GLfloat shininess = 20.0f;
	GLfloat strength = 10.0f;
	writeUniformBlockToShaderProgram(mesh_obj.shader_program, "uniforms", "ambient", ambient, 12, false);
	writeUniformBlockToShaderProgram(mesh_obj.shader_program, "uniforms", "diffuse", &diffuse, 4, false);
	writeUniformBlockToShaderProgram(mesh_obj.shader_program, "uniforms", "specular", &specular, 4, false);
	writeUniformBlockToShaderProgram(mesh_obj.shader_program, "uniforms", "shininess", &shininess, 4, false);
	writeUniformBlockToShaderProgram(mesh_obj.shader_program, "uniforms", "strength", &strength, 4, true);

	return mesh_obj;
}

void meshDrawSelf(MeshObj &mesh_obj)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh_obj.buffer);
	glBindVertexArray(mesh_obj.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh_obj.normal_offset / 3);
}
