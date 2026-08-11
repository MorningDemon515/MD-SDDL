#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h"
#include "Window.h"

#include <stdlib.h>

float mouse_wheel = 0.0f;

Handle M_W_Create(Manager* manager, int width, int height)
{
	Window* window = new Window();
	window->width = width; window->height = height;
	window->window = SDL_CreateWindow("", window->width, window->height, SDL_WINDOW_OPENGL);
	window->run = true;

	Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
	if (newRecords == NULL) return -1;

	manager->records = newRecords;
	int myId = manager->nextId++;

	manager->records[manager->size].id = myId;
	manager->records[manager->size].type = WINDOW; // Type !!!!!!!!
	manager->records[manager->size].ptr = window;

	manager->size += 1;
	return myId;
}

int W_IsRun(Window* window)
{
	return window->run;
}

void W_PollEvents(Window* window)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		     window->run = false;

		if (event.type == SDL_EVENT_MOUSE_WHEEL)
			mouse_wheel = event.wheel.y;
	}
}

void W_SetRun(Window* window, int run)
{
	window->run = run;
}

void W_GetSize(Window* window, int* width, int* height)
{
	SDL_GetWindowSize(window->window, width, height);
}

void W_SetRelative(Window* window, int relative)
{
	SDL_SetWindowRelativeMouseMode(window->window, relative);
}

void W_SetTitle(Window* window, const char* title)
{
	SDL_SetWindowTitle(window->window, title);
}

void W_SetSize(Window* window, int width, int height)
{
	SDL_SetWindowSize(window->window, width, height);
	SDL_SetWindowPosition(window->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	window->width = width; window->height = height;
}