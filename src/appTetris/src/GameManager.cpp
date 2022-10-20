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
	Subscribe();
	return SDLManagerPtr->Init();
}

void GameManager::Update()
{
	if (!SDLManagerPtr || !InputManagerPtr || !GameInstancePtr)
	{
		return;
	}

	GameInstancePtr->Play();
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