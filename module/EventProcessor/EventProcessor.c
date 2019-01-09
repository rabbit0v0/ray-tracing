#include "EventProcessor.h"

#include <math.h>

#define PI 3.14159265358979323846264338327950288419716939937510582097f

static char key_pressed_bitmap[32];
static float move_unit = 0.05;

void processEvent(SDL_Window *window, EventData *event_data)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
		{
			SDL_DestroyWindow(window);
			SDL_Quit();
			exit(0);
			break;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			processKeyboardEvent(e);
			break;
		}
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
		{
			processMouseEvent(e, event_data);
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void updateEventData(SDL_Window *window, EventData *event_data)
{
	if (ifKeyPressed(SDL_SCANCODE_LSHIFT))
	{
		move_unit = 0.005;
	}
	else if (ifKeyPressed(SDL_SCANCODE_LCTRL))
	{
		move_unit = 1;
	}
	else
	{
		move_unit = 0.05;
	}
	if (ifKeyPressed(SDL_SCANCODE_W))
	{
		event_data->eye_x += move_unit * cos(event_data->forward_h);
		event_data->eye_z += move_unit * sin(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_S))
	{
		event_data->eye_x -= move_unit * cos(event_data->forward_h);
		event_data->eye_z -= move_unit * sin(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_A))
	{
		event_data->eye_x += move_unit * sin(event_data->forward_h);
		event_data->eye_z -= move_unit * cos(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_D))
	{
		event_data->eye_x -= move_unit * sin(event_data->forward_h);
		event_data->eye_z += move_unit * cos(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_I))
	{
		event_data->light_x += move_unit * cos(event_data->forward_h);
		event_data->light_z += move_unit * sin(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_K))
	{
		event_data->light_x -= move_unit * cos(event_data->forward_h);
		event_data->light_z -= move_unit * sin(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_J))
	{
		event_data->light_x += move_unit * sin(event_data->forward_h);
		event_data->light_z -= move_unit * cos(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_L))
	{
		event_data->light_x -= move_unit * sin(event_data->forward_h);
		event_data->light_z += move_unit * cos(event_data->forward_h);
	}
	if (ifKeyPressed(SDL_SCANCODE_U))
	{
		event_data->light_y += move_unit;
	}
	if (ifKeyPressed(SDL_SCANCODE_O))
	{
		event_data->light_y -= move_unit;
	}
	if (ifKeyPressed(SDL_SCANCODE_R))
	{
		event_data->alarm = !event_data->alarm;
	}
	if (ifKeyPressed(SDL_SCANCODE_1))
	{
		event_data->drone_state = 0;
	}
	if (ifKeyPressed(SDL_SCANCODE_2))
	{
		event_data->drone_state = 1;
	}
	if (ifKeyPressed(SDL_SCANCODE_3))
	{
		event_data->drone_state = 2;
	}
	if (ifKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		exit(0);
	}

}

static void processKeyboardEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
	{
		pressKey(e.key.keysym.scancode);
		break;
	}
	case SDL_KEYUP:
	{
		releaseKey(e.key.keysym.scancode);
		break;
	}
	default:
	{
		break;
	}
	}
}

static void processMouseEvent(SDL_Event e, EventData *event_data)
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
	{
		static float epsilon = 0.001f;
		event_data->forward_v = fminf(event_data->forward_v - e.motion.yrel * 0.01f, PI / 2 - epsilon);
		event_data->forward_v = fmaxf(event_data->forward_v, -PI / 2 + epsilon);
		event_data->forward_h = fmodf(event_data->forward_h + e.motion.xrel * 0.01f, 2 * PI);
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	{
		break;
	}
	case SDL_MOUSEWHEEL:
	{
		event_data->eye_y += e.wheel.y * move_unit;
		break;
	}
	default:
	{
		break;
	}
	}
}

static void pressKey(SDL_Scancode key)
{
	key_pressed_bitmap[key / 8] |= 1 << (key % 8);
}

static void releaseKey(SDL_Scancode key)
{
	key_pressed_bitmap[key / 8] &= ~(1 << (key % 8));
}

static int ifKeyPressed(SDL_Scancode key)
{
	return key_pressed_bitmap[key / 8] & (1 << (key % 8));
}
