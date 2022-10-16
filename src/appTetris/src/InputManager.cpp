#include "../include/InputManager.h"

int InputManager::WaitPollEvent(SDL_Event& Event) const
{
	static constexpr int8_t Minus = -1;
	static constexpr int8_t Plus = 1;
	static constexpr int8_t Idle = 0;
	switch (Event.type)
	{
	case SDL_QUIT:
		QuitGameEvent(true);
		break;
	case SDL_KEYDOWN:
		switch (Event.key.type)
		{
		case SDLK_UP:
			DirectionalKeyPressedEvent(Idle, Plus);
			break;
		case SDLK_DOWN:
			DirectionalKeyPressedEvent(Idle, Minus);
			break;
		case SDLK_LEFT:
			DirectionalKeyPressedEvent(Minus, Idle);
			break;
		case SDLK_RIGHT:
			DirectionalKeyPressedEvent(Plus, Idle);
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