#include "EventProcessor.h"
#include "ShaderLoader.h"
#include "glad/glad.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_query.hpp>
#include <utility>

#define DEBUG 1

enum VAO_IDs
{
	Triangles,
	NumVAOs
};
enum Buffer_IDs
{
	ArrayBuffer,
	NumBuffers
};
enum Attrib_IDs
{
	vPosition = 0
};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLfloat pi = 3.141591653589793238462643383279502f;
GLfloat sphere[8][9][2][3];

glm::mat4 ModelS = glm::mat4(1.0f);
glm::mat4 ModelR = glm::mat4(1.0f);
glm::mat4 ModelT = glm::mat4(1.0f);

glm::mat4 clip_view = glm::perspective(pi / 2, 4.0f / 3.0f, 0.01f, 100.0f);

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
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

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	loadShader("default.vert");
	loadShader("default.frag");
	useProgram();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat z_last = -1;
	for (int i = 0; i < 8; i++)
	{
		GLfloat z = sin(((i + 1.0f - 4.0f) / 8.0f) * pi);
		for (int j = 0; j < 9; j++)
		{
			GLfloat x = cos((j / 4.0f) * pi);
			GLfloat y = sin((j / 4.0f) * pi);
			sphere[i][j][0][0] = x * cos(((i - 4.0f) / 9.0f) * pi);
			sphere[i][j][0][2] = y * cos(((i - 4.0f) / 9.0f) * pi);
			sphere[i][j][0][1] = z_last;
			sphere[i][j][1][0] = x * cos(((i + 1.0f - 4.0f) / 9.0f) * pi);
			sphere[i][j][1][2] = y * cos(((i + 1.0f - 4.0f) / 9.0f) * pi);
			sphere[i][j][1][1] = z;
		}
		z_last = z;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
}

void display(void)
{
	GLfloat bg_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, bg_color);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 View = glm::lookAt(glm::vec3(eye_x, eye_y, eye_z),
								 glm::vec3(eye_x, eye_y, eye_z) + glm::vec3(cos(forward_h), forward_v, sin(forward_h)),
								 glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = ModelT * ModelR * ModelS;

	glm::mat4 clip_model = clip_view * View * Model;
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(clip_model));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8 * 9 * 2);
}
