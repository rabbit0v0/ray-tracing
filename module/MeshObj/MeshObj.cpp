#include "MeshObj.h"

#define PRINT_LINE printf("%s: %d\n", __FILE__, __LINE__)

enum Attrib_IDs
{
	VertexPosition = 0,
	VertexNormal,
	VertexColor,
};

int meshCreate(tinyobj::attrib_t *attrib, std::vector<tinyobj::shape_t> *shapes,
			   std::vector<tinyobj::material_t> *materials, MeshObj *mesh_obj)
{
	glGenBuffers(1, &(mesh_obj->buffer));
	glGenVertexArrays(1, &(mesh_obj->vao));
	glBindBuffer(GL_ARRAY_BUFFER, mesh_obj->buffer);
	glBindVertexArray(mesh_obj->vao);

	GLfloat *temp_v_buffer = (GLfloat *)malloc(200 * attrib->vertices.size() * sizeof(tinyobj::real_t));
	GLfloat *temp_n_buffer = (GLfloat *)malloc(200 * attrib->normals.size() * sizeof(tinyobj::real_t));
	GLfloat *temp_t_buffer = (GLfloat *)malloc(200 * attrib->texcoords.size() * sizeof(tinyobj::real_t));
	int v_offset = 0;
	int n_offset = 0;
	int t_offset = 0;

	for (size_t s = 0; s < shapes->size(); s++)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < (*shapes)[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = (*shapes)[s].mesh.num_face_vertices[f];
			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = (*shapes)[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib->vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib->vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib->vertices[3 * idx.vertex_index + 2];
				temp_v_buffer[v_offset] = vx;
				temp_v_buffer[v_offset + 1] = vy;
				temp_v_buffer[v_offset + 2] = vz;
				v_offset += 3;

				tinyobj::real_t nx = attrib->normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib->normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib->normals[3 * idx.normal_index + 2];

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
			//			(*shapes)[s].mesh.material_ids[f];
		}
	}

	glBufferData(GL_ARRAY_BUFFER, (v_offset + n_offset + t_offset) * sizeof(tinyobj::real_t), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v_offset * sizeof(tinyobj::real_t), temp_v_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, v_offset * sizeof(tinyobj::real_t), n_offset * sizeof(tinyobj::real_t),
					temp_n_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, (v_offset + n_offset) * sizeof(tinyobj::real_t),
					t_offset * sizeof(tinyobj::real_t), temp_t_buffer);
	mesh_obj->normal_offset = v_offset;
	mesh_obj->texcoord_offset = v_offset + n_offset;
	free(temp_v_buffer);
	free(temp_n_buffer);
	free(temp_t_buffer);
	glVertexAttribPointer(VertexPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VertexPosition);
	glVertexAttribPointer(VertexNormal, 3, GL_FLOAT, GL_FALSE, 0,
						  (const GLvoid *)((long)mesh_obj->normal_offset * sizeof(tinyobj::real_t)));
	glEnableVertexAttribArray(VertexNormal);
	glVertexAttrib4f(VertexColor, 1.0f, 1.0f, 1.0f, 1.0f);
	return 0;
}

void meshDrawSelf(MeshObj &mesh_obj)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh_obj.buffer);
	glBindVertexArray(mesh_obj.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh_obj.normal_offset / 3);
}
