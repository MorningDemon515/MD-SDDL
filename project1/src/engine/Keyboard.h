#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL3/SDL.h>

typedef struct Key
{
	const bool* keyborads;
	int keyNum;
	bool lastKey[SDL_SCANCODE_COUNT] = {0};
} Key;

#endif