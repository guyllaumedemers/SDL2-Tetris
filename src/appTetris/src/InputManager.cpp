#include "../include/InputManager.h"
#include <cstdlib>

int InputManager::WaitPollEvent(SDL_Event& Event)
{
	switch (Event.type)
	{
	case SDL_QUIT:
		QuitGameEvent(true);
		break;
	case SDL_KEYDOWN:
		switch (Event.key.type)
		{
		case SDLK_UP:
			DirectionalKeyPressedEvent(0, 1);
			break;
		case SDLK_DOWN:
			DirectionalKeyPressedEvent(0, -1);
			break;
		case SDLK_LEFT:
			DirectionalKeyPressedEvent(-1, 0);
			break;
		case SDLK_RIGHT:
			DirectionalKeyPressedEvent(1, 0);
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