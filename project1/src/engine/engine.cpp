#define ENGINE_DLL
#include "engine.h"

#include <iostream>
#include <SDL3/SDL.h>

void InitEngine()
{
	SDL_Init(SDL_INIT_VIDEO);
}

void QuitEngine()
{
	SDL_Quit();
}

float GetTime()
{
	return (float)SDL_GetTicks();
}

void Delay(unsigned int time)
{
	SDL_Delay(time);
}

void Test()
{

}