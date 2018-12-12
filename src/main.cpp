#include "EventProcessor.h"
#include "ShaderLoader.h"
#include "glad/glad.h"
#include "ShaderProgram.h"
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
#include <vector>

#define DEBUG 0
#define PRINT_LINE(msg) printf("line: %d: %s\n", __LINE__, msg)

std::vector<MeshObj> meshes;

void init(ShaderProgram *shader_program);
void display(EventData event_data, ShaderProgram *shader_program);
int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_Window *window = SDL_CreateWindow("openGL demo", 0, 0, 256, 256, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	// SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("OpenGL Version %s loaded.\n", glGetString(GL_VERSION));
	ShaderProgram shader_program;
	init(&shader_program);
	EventData event_data = {0};
	while (true)
	{
		processEvent(window, &event_data);
		updateEventData(window, &event_data);
		display(event_data, &shader_program);
		SDL_GL_SwapWindow(window);
	}
}

void init(ShaderProgram *shader_program)
{
	glEnable(GL_DEPTH_TEST);
	ShaderObj vert_shader = loadShader("default.vert");
	ShaderObj frag_shader = loadShader("default.frag");
	GLuint program = glCreateProgram();
	attachProgram(program, vert_shader);
	attachProgram(program, frag_shader);
	useProgram(program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	createShaderProgram(program, shader_program);
	MeshObj mesh_obj = meshCreate(shader_program, "./res/scene.obj", "./res/");
	meshes.push_back(mesh_obj);
}

void display(EventData event_data, ShaderProgram *shader_program)
{
	GLfloat bg_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bg_color);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 View =
		glm::lookAt(glm::vec3(event_data.eye_x, event_data.eye_y, event_data.eye_z),
					glm::vec3(event_data.eye_x, event_data.eye_y, event_data.eye_z) +
						glm::vec3(cos(event_data.forward_h), event_data.forward_v, sin(event_data.forward_h)),
					glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 clip_view = glm::perspective(1.570796327f, 4.0f / 3.0f, 0.01f, 100.0f);
	glm::mat4 clip_model = clip_view * View;
	writeUniformToShaderProgram(shader_program, "clip_model", glm::value_ptr(clip_model), mat4fv);

	GLfloat data_light_color[] = {1.0f, 1.0f, 1.0f};
	GLfloat data_eye_direction[] = {cos(event_data.forward_h), event_data.forward_v, sin(event_data.forward_h)};
	GLfloat data_light_pos[] = {event_data.light_x, event_data.light_y, event_data.light_z};
	writeUniformBlockToShaderProgram(shader_program, "uniforms", "eye_direction", data_eye_direction, 12, false);
	writeUniformBlockToShaderProgram(shader_program, "uniforms", "light_pos", data_light_pos, 12, false);
	writeUniformBlockToShaderProgram(shader_program, "uniforms", "light_color", data_light_color, 12, true);

	int i;
	for (i = 0; i < meshes.size(); i++)
	{
		meshDrawSelf(meshes[i]);
	}
}
