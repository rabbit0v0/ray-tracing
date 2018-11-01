#ifndef __EventProcessor_h_
#define __EventProcessor_h_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

/*
 * The main processor
 */
void processEvent();

/*
 * This field list sub processors that process different kind of events
 */
static void processKeyboardEvent(SDL_Event e);
static void processMouseEvent(SDL_Event e);

/*
 * Help function that record and report the key state
 */
static void pressKey(SDL_Scancode key);
static void releaseKey(SDL_Scancode key);
static int ifKeyPressed(SDL_Scancode key);








#endif
