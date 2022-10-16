#include "../include/GameManager.h"
#include "../include/SDLManager.h"
#include "../include/InputManager.h"

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
	if (!SDLManagerPtr || !InputManagerPtr || !GameInstancePtr)
	{
		return EXIT_FAILURE;
	}
	InputManagerPtr->QuitGameEvent = [&](bool bHasQuitGame) { bIsQuittingGame = bHasQuitGame; };
	InputManagerPtr->DirectionalKeyPressedEvent = [&](int DirX, int DirY) { GameInstancePtr->PollKeyEvent(DirX, DirY); };
	return SDLManagerPtr->Init();
}

void GameManager::Update()
{
	if (!SDLManagerPtr || !InputManagerPtr || !GameInstancePtr)
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
		/*Workaround capture clause problem when used in conjunction with void(*)*/
		SDLManagerPtr->Update([](GameInstance* const GameInstanceArgPtr) { GameInstanceArgPtr->Update(); }, GameInstancePtr.get());
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