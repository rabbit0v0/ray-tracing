#include "EventProcessor.h"

static char key_pressed_bitmap[32];

void processEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
			{
				break;
			}
			case SDL_APP_TERMINATING:
			case SDL_APP_LOWMEMORY:
			case SDL_APP_WILLENTERBACKGROUND:
			case SDL_APP_DIDENTERBACKGROUND:
			case SDL_APP_DIDENTERFOREGROUND:
			{
				break;
			}
			case SDL_WINDOWEVENT:
			case SDL_SYSWMEVENT:
			{
				break;
			}
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTEDITING:
			case SDL_TEXTINPUT:
			case SDL_KEYMAPCHANGED:
			{
				break;
			}
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			{
				break;
			}
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
			{
				break;
			}
			case SDL_CONTROLLERAXISMOTION:
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMOVED:
			case SDL_CONTROLLERDEVICEREMAPPED:
			{
				break;
			}
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
			case SDL_FINGERMOTION:
			{
				break;
			}
			case SDL_DOLLARGESTURE:
			case SDL_DOLLARRECORD:
			case SDL_MULTIGESTURE:
			{
				break;
			}
			case SDL_CLIPBOARDUPDATE:
			{
				break;
			}
			case SDL_DROPFILE:
			case SDL_DROPTEXT:
			case SDL_DROPBEGIN:
			case SDL_DROPCOMPLETE:
			{
				break;
			}
			case SDL_AUDIODEVICEADDED:
			case SDL_AUDIODEVICEREMOVED:
			{
				break;
			}
			case SDL_RENDER_TARGETS_RESET:
			case SDL_RENDER_DEVICE_RESET:
			{
				break;
			}
			case SDL_USEREVENT:
			case SDL_LASTEVENT:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

static void processKeyboardEvent(SDL_Event e)
{
}

static void processMouseEvent(SDL_Event e)
{
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
