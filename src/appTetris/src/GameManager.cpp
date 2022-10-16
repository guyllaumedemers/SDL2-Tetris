#include "../include/GameManager.h"
#include "../include/SDLManager.h"
#include "../include/InputManager.h"

#include <string>

int GameManager::Run()
{
	if (Init() != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	Update();
	Clear();
	return EXIT_SUCCESS;
}

int GameManager::Init()
{
	if (!SDLManagerPtr || !InputManagerPtr)
	{
		return EXIT_FAILURE;
	}
	InputManagerPtr->QuitGameEvent = [&](bool bHasQuitGame) { bIsQuittingGame = bHasQuitGame; };
	InputManagerPtr->DirectionalKeyPressedEvent = [&](int DirX, int DirY) { std::sprintf("{0} {1}", std::to_string(DirX).c_str(), std::to_string(DirY).c_str()); /*Invoke game logic for Tetrominos movement*/ };
	return SDLManagerPtr->Init();
}

void GameManager::Update()
{
	if (!SDLManagerPtr || !InputManagerPtr)
	{
		return;
	}

	bIsQuittingGame = false;
	while (!bIsQuittingGame)
	{
		SDL_Event Event;
		if (SDL_WaitEvent(&Event) > NULL)
		{
			InputManagerPtr->WaitPollEvent(Event);
		}
		SDLManagerPtr->Update();
	}
}

void GameManager::Clear()
{
	if (!SDLManagerPtr)
	{
		return;
	}
	SDLManagerPtr->Clear();
}