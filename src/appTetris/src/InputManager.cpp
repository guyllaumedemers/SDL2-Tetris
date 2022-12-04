#include "../include/InputManager.h"

int InputManager::RunEvent(SDL_Event& Event) const
{
	static constexpr int8_t&& Minus = -1;
	static constexpr int8_t&& Plus = 1;
	static constexpr int8_t&& Idle = 0;
	switch (Event.type)
	{
	case SDL_QUIT:
		QuitGameEvent(true);
		break;
	case SDL_KEYDOWN:
		switch (Event.key.keysym.scancode)
		{
		case SDL_SCANCODE_UP:
			DirectionalKeyPressedEvent(Idle, Plus);
			break;
		case SDL_SCANCODE_DOWN:
			DirectionalKeyPressedEvent(Idle, Minus);
			break;
		case SDL_SCANCODE_LEFT:
			DirectionalKeyPressedEvent(Minus, Idle);
			break;
		case SDL_SCANCODE_RIGHT:
			DirectionalKeyPressedEvent(Plus, Idle);
			break;
		case SDL_SCANCODE_SPACE:
			DelSpaceKeyPressedEvent();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return EXIT_SUCCESS;
}