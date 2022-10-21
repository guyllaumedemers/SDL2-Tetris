#include "../include/GameManager.h"
#include "../include/SDLManager.h"
#include "../include/InputManager.h"

int GameManager::Run()
{
	Init();
	Update();
	Clear();
	return EXIT_SUCCESS;
}

void GameManager::Init()
{
	if (!SDLManagerPtr || !InputManagerPtr || !GameInstancePtr)
	{
		return;
	}
	Subscribe();
	SDLManagerPtr->Init();
	GameInstancePtr->Play();
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
		SDLManagerPtr->Update([&]() { GameInstancePtr->Update(); });
	}
}

void GameManager::Clear()
{
	if (!SDLManagerPtr)
	{
		return;
	}
	SDLManagerPtr->Clear();
	UnSubscribe();
}

void GameManager::Subscribe()
{
	InputManagerPtr->QuitGameEvent = [&](bool bHasQuitGame)
	{
		bIsQuittingGame = bHasQuitGame;
	};
	InputManagerPtr->DirectionalKeyPressedEvent = [&](int8_t DirX, int8_t DirY)
	{
		GameInstancePtr->PollKeyEvent(DirX, DirY);
	};
	InputManagerPtr->DelSpaceKeyPressedEvent = [&]()
	{
		TetrominoeManager* const TetrominoeManagerPtr = GameInstancePtr->TetrominoeManagerPtr.get();
		if (!TetrominoeManagerPtr)
		{
			return;
		}
		TetrominoeManagerPtr->Flip();
	};
	GameInstancePtr->SetWindowEvent = [&](uint16_t Width, uint16_t Height)
	{
		SDLManagerPtr->SetWindowContextSize(Width, Height);
	};
}

void GameManager::UnSubscribe()
{
	InputManagerPtr->QuitGameEvent = nullptr;
	InputManagerPtr->DirectionalKeyPressedEvent = nullptr;
	InputManagerPtr->DelSpaceKeyPressedEvent = nullptr;
	GameInstancePtr->SetWindowEvent = nullptr;
}