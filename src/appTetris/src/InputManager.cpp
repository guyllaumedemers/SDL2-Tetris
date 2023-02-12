#include "../include/InputManager.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

InputManager::~InputManager()
{
	DirectionalKeyPressedDelegate = nullptr;
	SpaceKeyPressedDelegtate = nullptr;
	QuitGameDelegate = nullptr;
}

void InputManager::RunEvent(SDL_Event* Event) const
{
	if (!Event)
	{
		return;
	}

	static constexpr int8_t&& Minus = -1;
	static constexpr int8_t&& Plus = 1;
	static constexpr int8_t&& Idle = 0;
	switch (Event->type)
	{
	case SDL_QUIT:
		QuitGameDelegate(true);
		break;
	case SDL_KEYDOWN:
		switch (Event->key.keysym.scancode)
		{
		case SDL_SCANCODE_UP:
			DirectionalKeyPressedDelegate(Idle, Idle);
			break;
		case SDL_SCANCODE_DOWN:
			DirectionalKeyPressedDelegate(Idle, Minus);
			break;
		case SDL_SCANCODE_LEFT:
			DirectionalKeyPressedDelegate(Minus, Idle);
			break;
		case SDL_SCANCODE_RIGHT:
			DirectionalKeyPressedDelegate(Plus, Idle);
			break;
		case SDL_SCANCODE_SPACE:
			SpaceKeyPressedDelegtate();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}