#ifndef __ShaderProgram_h_
#define __ShaderProgram_h_
#include "glad/glad.h"
#include <string>
#include <map>

enum UniformWriteMode
{
	f1,
	f2,
	f3,
	f4,
	i1,
	i2,
	i3,
	i4,
	ui1,
	ui2,
	ui3,
	ui4,
	fv1,
	fv2,
	fv3,
	fv4,
	iv1,
	iv2,
	iv3,
	iv4,
	uiv1,
	uiv2,
	uiv3,
	uiv4,
	mat2fv,
	mat3fv,
	mat4fv,
	mat2x3fv,
	mat3x2fv,
	mat2x4fv,
	mat4x2fv,
	mat3x4fv,
	mat4x3fv
};

typedef struct ShaderProgram_ ShaderProgram;
typedef struct Uniform_ Uniform;
typedef struct UniformBlock_ UniformBlock;

typedef struct ShaderProgram_
{
	GLuint program;
	std::map<std::string, Uniform> uniforms;			// map uniform names to locations
	std::map<std::string, UniformBlock> uniform_blocks; // map uniform block names to UniformBlock
} ShaderProgram;

typedef struct Uniform_
{
	GLuint uniform_location;
} Uniform;

typedef struct UniformBlock_
{
	GLuint block_index;
	GLint block_size;
	GLuint buffer_obj;
	std::map<std::string, Uniform> uniform_vars; // map uniform names to Uniform
	void *buffer_data;
} UniformBlock;

void createShaderProgram(GLuint program, ShaderProgram *shader_program);
void destroyShaderProgram(ShaderProgram *shader_program);

int writeUniformToShaderProgram(ShaderProgram *shader_program, std::string uniform_name, GLvoid *data,
								UniformWriteMode write_mode);

int writeUniformBlockToShaderProgram(ShaderProgram *shader_program, std::string uniform_block_name,
									 std::string uniform_name, GLvoid *data, int bytes, int flush);

void createUniformBlock(GLuint program, std::string uniform_block_name, const char *uniform_var_names[],
						int num_of_uniform_vars, UniformBlock *uniform_block);
void destroyUniformBlock(UniformBlock *uniform_block);

#endif
