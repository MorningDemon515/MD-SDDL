#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

typedef struct Window
{
	SDL_Window* window;
	int width, height;
	bool run;
} Window;

#endif