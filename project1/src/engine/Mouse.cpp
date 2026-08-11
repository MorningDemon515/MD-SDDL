#define ENGINE_DLL
#include "engine.h"
#include "Mouse.h"
#include "Window/Window.h"

#include <SDL3/SDL.h>

static float m_x, m_y;
static SDL_MouseButtonFlags mouseState;
static SDL_MouseButtonFlags lastMouseState;

void Mouse_Update()
{
	memcpy(&lastMouseState, &mouseState, sizeof(mouseState));
	mouseState = SDL_GetMouseState(&m_x, &m_y);
}

void Mouse_GetPos(float* x, float* y)
{
	*x = m_x; *y = m_y;
}

void Mouse_GetRelative(float* x, float* y)
{
	SDL_GetRelativeMouseState(x, y);
}

float Mouse_GetWheel()
{
	return mouse_wheel;
}

void Mouse_SetPos(Window* window, float x,float y)
{
	SDL_WarpMouseInWindow(window->window, x, y);
}

int Mouse_Press(MouseButton mb)
{
	return (mouseState & SDL_BUTTON_MASK(mb)) && !(lastMouseState & SDL_BUTTON_MASK(mb));
}

int Mouse_Down(MouseButton mb)
{
	return mouseState & SDL_BUTTON_MASK(mb);
}

int Mouse_Up(MouseButton mb)
{
	return !(mouseState & SDL_BUTTON_MASK(mb)) && lastMouseState & SDL_BUTTON_MASK(mb);
}