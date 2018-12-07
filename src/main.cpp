#include "EventProcessor.h"
#include "ShaderLoader.h"
#include "glad/glad.h"
#include "tiny_obj_loader.h"
#include "MeshObj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_query.hpp>
#include <utility>
#include <iostream>

#define DEBUG 0
#define PRINT_LINE(msg) printf("line: %d: %s\n", __LINE__, msg)

enum UniformLocation
{
	ambient,
	light_color,
	light_direction,
	eye_direction,
	shininess,
	strength,
	light_pos,
	NumUniformLocation
};

GLuint uniform_block_mtl_indices[NumUniformLocation];
GLint uniform_block_mtl_size[NumUniformLocation];
GLint uniform_block_mtl_offset[NumUniformLocation];
GLint uniform_block_mtl_type[NumUniformLocation];
GLint uniform_size;
GLuint ubo;
GLvoid *uniform_mtl_buffer;
GLuint uniform_index;

GLuint clip_model_index;

glm::mat4 clip_view = glm::perspective(1.570796327f, 4.0f / 3.0f, 0.01f, 100.0f);
std::vector<MeshObj> meshes;

#if DEBUG
static void APIENTRY simple_print_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
										   const GLchar *message, const void *userParam)
{
	printf("Debug message  '%s'\n", message);
}
#endif

void init(void);
void display(void);
int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_Window *window = SDL_CreateWindow("openGL demo", 0, 0, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

#if DEBUG
	printf("OpenGL Version %s loaded.\n", glGetString(GL_VERSION));
#endif

	init();
	while (true)
	{
		processEvent(window);
		display();
		SDL_GL_SwapWindow(window);
	}
}

void init(void)
{
#if DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(simple_print_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
	glEnable(GL_DEPTH_TEST);

	loadShader("default.vert");
	loadShader("default.frag");
	GLuint program;
	useProgram(&program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "./res/scene.obj", "./res/");
	if (!err.empty())
	{
		std::cerr << err << std::endl;
	}
	if (!ret)
	{
		exit(1);
	}
	MeshObj test_obj;
	meshCreate(&attrib, &shapes, &materials, &test_obj);
	meshes.push_back(test_obj);

	uniform_index = glGetUniformBlockIndex(program, "mtl");

	const char *uniform_names[NumUniformLocation] = {"ambient",   "light_color", "light_direction", "eye_direction",
													 "shininess", "strength",	"light_pos"};

	glGetActiveUniformBlockiv(program, uniform_index, GL_UNIFORM_BLOCK_DATA_SIZE, &uniform_size);
	uniform_mtl_buffer = malloc(uniform_size);
	glGetUniformIndices(program, NumUniformLocation, uniform_names, uniform_block_mtl_indices);
	glGetActiveUniformsiv(program, NumUniformLocation, uniform_block_mtl_indices, GL_UNIFORM_OFFSET,
						  uniform_block_mtl_offset);
	glGetActiveUniformsiv(program, NumUniformLocation, uniform_block_mtl_indices, GL_UNIFORM_SIZE,
						  uniform_block_mtl_size);
	glGetActiveUniformsiv(program, NumUniformLocation, uniform_block_mtl_indices, GL_UNIFORM_TYPE,
						  uniform_block_mtl_type);
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, uniform_size, NULL, GL_STATIC_DRAW);

	clip_model_index = glGetUniformLocation(program, "clip_model");

	glDeleteProgram(program);
}

void display(void)
{
	GLfloat bg_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bg_color);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 View = glm::lookAt(glm::vec3(eye_x, eye_y, eye_z),
								 glm::vec3(eye_x, eye_y, eye_z) + glm::vec3(cos(forward_h), forward_v, sin(forward_h)),
								 glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 clip_model = clip_view * View;
	glUniformMatrix4fv(clip_model_index, 1, GL_FALSE, glm::value_ptr(clip_model));

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	GLfloat data_ambient[] = {0.3f, 0.3f, 0.3f};
	GLfloat data_light_direction[] = {0.0f, 0.0f, 0.0f};
	GLfloat data_light_color[] = {1.0f, 0.0f, 0.0f};
	GLfloat data_eye_direction[] = {cos(forward_h), forward_v, sin(forward_h)};
	GLfloat data_shininess = 20.0f;
	GLfloat data_strength = 0.5f;
	GLfloat data_light_pos[] = {light_x, light_y, light_z};

	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[ambient]), &data_ambient, 12);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[light_direction]), &data_light_direction, 12);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[light_color]), &data_light_color, 12);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[eye_direction]), &data_eye_direction, 12);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[shininess]), &data_shininess, 4);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[strength]), &data_strength, 4);
	memcpy((void *)((long)uniform_mtl_buffer + uniform_block_mtl_offset[light_pos]), &data_light_pos, 12);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, uniform_size, uniform_mtl_buffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniform_index, ubo);

	meshDrawSelf(meshes[0]);
}
