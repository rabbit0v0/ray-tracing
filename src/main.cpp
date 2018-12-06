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
	clipmodel,
	ambient,
	light_color,
	light_direction,
	eye_direction,
	shininess,
	strength,
	light_pos,
	NumUniformLocation
};

GLuint UniformLocations[NumUniformLocation];

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


	UniformLocations[clipmodel] = glGetUniformLocation(program, "clip_model");
	UniformLocations[ambient] = glGetUniformLocation(program, "ambient");
	UniformLocations[light_color] = glGetUniformLocation(program, "light_color");
	UniformLocations[light_direction] = glGetUniformLocation(program, "light_direction");
	UniformLocations[eye_direction] = glGetUniformLocation(program, "eye_direction");
	UniformLocations[shininess] = glGetUniformLocation(program, "shininess");
	UniformLocations[strength] = glGetUniformLocation(program, "strength");
	UniformLocations[light_pos] = glGetUniformLocation(program, "light_pos");
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

	glUniformMatrix4fv(UniformLocations[clipmodel], 1, GL_FALSE, glm::value_ptr(clip_model));

	glUniform3f(UniformLocations[ambient], 0.3f, 0.3f, 0.3f);
	glUniform3f(UniformLocations[light_direction], 0.0f, 0.5f, 0.5f);
	glUniform3f(UniformLocations[light_color], 0.0f, 0.5f, 0.5f);
	glUniform3f(UniformLocations[eye_direction], cos(forward_h), forward_v, sin(forward_h));
	glUniform1f(UniformLocations[shininess], 20.0f);
	glUniform1f(UniformLocations[strength], 0.5f);
	glUniform3f(UniformLocations[light_pos], light_x, light_y, light_z);

	meshDrawSelf(meshes[0]);
}
