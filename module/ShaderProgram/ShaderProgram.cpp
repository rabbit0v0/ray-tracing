#include "ShaderProgram.h"
#include <string.h>

#define PRINT_LINE printf("%s: %d\n", __FILE__, __LINE__)

// still hard coding, to test
void createShaderProgram(GLuint program, ShaderProgram *shader_program)
{
	shader_program->program = program;

	shader_program->uniforms["clip_model"].uniform_location = glGetUniformLocation(program, "clip_model");
	const char *uniforms_uniform_var_names[] = {"eye_direction", "light_pos", "light_color", "ambient", "diffuse", "specular", "shininess", "strength"};
	createUniformBlock(program, "uniforms", uniforms_uniform_var_names, 8, &(shader_program->uniform_blocks["uniforms"]));
}

void destroyShaderProgram(ShaderProgram *shader_program)
{
}

int writeUniformToShaderProgram(ShaderProgram *shader_program, std::string uniform_name, GLvoid *data,
								UniformWriteMode write_mode)
{
	if (shader_program->uniforms.find(uniform_name) == shader_program->uniforms.end())
	{
		return 1;
	}
	switch (write_mode)
	{
	case f1:
		glUniform1f(shader_program->uniforms[uniform_name].uniform_location, *(GLfloat *)data);
		break;
	case f2:
		glUniform2f(shader_program->uniforms[uniform_name].uniform_location, *(GLfloat *)data, *((GLfloat *)data + 1));
		break;
	case f3:
		glUniform3f(shader_program->uniforms[uniform_name].uniform_location, *(GLfloat *)data, *((GLfloat *)data + 1),
					*((GLfloat *)data + 2));
		break;
	case f4:
		glUniform4f(shader_program->uniforms[uniform_name].uniform_location, *(GLfloat *)data, *((GLfloat *)data + 1),
					*((GLfloat *)data + 2), *((GLfloat *)data + 3));
		break;
	case i1:
		glUniform1i(shader_program->uniforms[uniform_name].uniform_location, *(GLint *)data);
		break;
	case i2:
		glUniform2i(shader_program->uniforms[uniform_name].uniform_location, *(GLint *)data, *((GLint *)data + 1));
		break;
	case i3:
		glUniform3i(shader_program->uniforms[uniform_name].uniform_location, *(GLint *)data, *((GLint *)data + 1),
					*((GLint *)data + 2));
		break;
	case i4:
		glUniform4i(shader_program->uniforms[uniform_name].uniform_location, *(GLint *)data, *((GLint *)data + 1),
					*((GLint *)data + 2), *((GLint *)data + 3));
		break;
	case ui1:
		glUniform1ui(shader_program->uniforms[uniform_name].uniform_location, *(GLuint *)data);
		break;
	case ui2:
		glUniform2ui(shader_program->uniforms[uniform_name].uniform_location, *(GLuint *)data, *((GLuint *)data + 1));
		break;
	case ui3:
		glUniform3ui(shader_program->uniforms[uniform_name].uniform_location, *(GLuint *)data, *((GLuint *)data + 1),
					 *((GLuint *)data + 2));
		break;
	case ui4:
		glUniform4ui(shader_program->uniforms[uniform_name].uniform_location, *(GLuint *)data, *((GLuint *)data + 1),
					 *((GLuint *)data + 2), *((GLuint *)data + 3));
		break;
	case fv1:
		glUniform1fv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLfloat *)data);
		break;
	case fv2:
		glUniform2fv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLfloat *)data);
		break;
	case fv3:
		glUniform3fv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLfloat *)data);
		break;
	case fv4:
		glUniform4fv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLfloat *)data);
		break;
	case iv1:
		glUniform1iv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLint *)data);
		break;
	case iv2:
		glUniform2iv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLint *)data);
		break;
	case iv3:
		glUniform3iv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLint *)data);
		break;
	case iv4:
		glUniform4iv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLint *)data);
		break;
	case uiv1:
		glUniform1uiv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLuint *)data);
		break;
	case uiv2:
		glUniform2uiv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLuint *)data);
		break;
	case uiv3:
		glUniform3uiv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLuint *)data);
		break;
	case uiv4:
		glUniform4uiv(shader_program->uniforms[uniform_name].uniform_location, 1, (GLuint *)data);
		break;
	case mat2fv:
		glUniformMatrix2fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat3fv:
		glUniformMatrix3fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat4fv:
		glUniformMatrix4fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat2x3fv:
		glUniformMatrix2x3fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat3x2fv:
		glUniformMatrix3x2fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat2x4fv:
		glUniformMatrix2x4fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat4x2fv:
		glUniformMatrix4x2fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat3x4fv:
		glUniformMatrix3x4fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	case mat4x3fv:
		glUniformMatrix4x3fv(shader_program->uniforms[uniform_name].uniform_location, 1, GL_FALSE, (GLfloat *)data);
		break;
	default:
	{
		break;
	}
	}
	return 0;
}

int writeUniformBlockToShaderProgram(ShaderProgram *shader_program, std::string uniform_block_name,
									 std::string uniform_name, GLvoid *data, int bytes, int flush)
{
	if (shader_program->uniform_blocks.find(uniform_block_name) == shader_program->uniform_blocks.end())
	{
		return 1;
	}
	if (shader_program->uniform_blocks[uniform_block_name].uniform_vars.find(uniform_name) ==
		shader_program->uniform_blocks[uniform_block_name].uniform_vars.end())
	{
		return 2;
	}
	memcpy((void *)((long)shader_program->uniform_blocks[uniform_block_name].buffer_data +
					shader_program->uniform_blocks[uniform_block_name].uniform_vars[uniform_name].uniform_location),
		   data, bytes);
	if (flush)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, shader_program->uniform_blocks[uniform_block_name].buffer_obj);
		glBufferData(GL_UNIFORM_BUFFER, shader_program->uniform_blocks[uniform_block_name].block_size,
					 shader_program->uniform_blocks[uniform_block_name].buffer_data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader_program->uniform_blocks[uniform_block_name].block_index,
						 shader_program->uniform_blocks[uniform_block_name].buffer_obj);
	}
	return 0;
}

void createUniformBlock(GLuint program, std::string uniform_block_name, const char *uniform_var_names[],
						int num_of_uniform_vars, UniformBlock *uniform_block)
{
	uniform_block->block_index = glGetUniformBlockIndex(program, uniform_block_name.c_str());
	glGenBuffers(1, &(uniform_block->buffer_obj));
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_block->buffer_obj);
	GLint block_size;
	glGetActiveUniformBlockiv(program, uniform_block->block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
	uniform_block->block_size = block_size;
	uniform_block->buffer_data = malloc(block_size);
	GLuint indices[num_of_uniform_vars];
	GLint offset[num_of_uniform_vars];
	glGetUniformIndices(program, num_of_uniform_vars, uniform_var_names, indices);
	glGetActiveUniformsiv(program, num_of_uniform_vars, indices, GL_UNIFORM_OFFSET, offset);
	int i;
	for (i = 0; i < num_of_uniform_vars; i++)
	{
		std::string tmp = uniform_var_names[i];
		uniform_block->uniform_vars[tmp].uniform_location = (GLuint)offset[i];
	}
}

void destroyUniformBlock(UniformBlock *uniform_block)
{
	glDeleteBuffers(1, &(uniform_block->buffer_obj));
	free(uniform_block->buffer_data);
}
