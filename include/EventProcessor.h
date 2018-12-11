#ifndef __EventProcessor_h_
#define __EventProcessor_h_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EventData_
{
	float eye_x;
	float eye_y;
	float eye_z;
	float forward_v;
	float forward_h;
	float light_x;
	float light_y;
	float light_z;
} EventData;

/*
 * The main processor
 */
void processEvent(SDL_Window *window, EventData *event_data);
void updateEventData(SDL_Window *window, EventData *event_data);

/*
 * This field list sub processors that process different kind of events
 */
static void processKeyboardEvent(SDL_Event e);
static void processMouseEvent(SDL_Event e, EventData *event_data);

/*
 * Help function that record and report the key state
 */
static void pressKey(SDL_Scancode key);
static void releaseKey(SDL_Scancode key);
static int ifKeyPressed(SDL_Scancode key);

#ifdef __cplusplus
}
#endif
#endif
