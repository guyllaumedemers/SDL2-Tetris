#include "../include/GameManager.h"

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
	TextureManagerPtr->Init(SDLManagerPtr.get());
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
		SDLManagerPtr->Update(
			TextureManagerPtr.get(),
			[&](TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg)
			{
				if (!TextureManagerPtrArg)
				{
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_MANAGER_PTR INVALID IN SDL_MANAGER_UPDATE!");
					return;
				}

				if (!SDLManagerPtrArg)
				{
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN SDL_MANAGER_UPDATE!");
					return;
				}

				GameInstancePtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
			});
	}
}

void GameManager::Clear()
{
	if (!SDLManagerPtr || !TextureManagerPtr || !GameInstancePtr)
	{
		return;
	}

	SDLManagerPtr->Clear();
	TextureManagerPtr->Clear();
	GameInstancePtr->Quit();
	UnSubscribe();
}

void GameManager::Subscribe()
{
	if (!GameInstancePtr || !InputManagerPtr)
	{
		return;
	}

	InputManagerPtr->QuitGameEvent = [&](bool bHasQuitGame)
	{
		bIsQuittingGame = bHasQuitGame;
	};

	InputManagerPtr->DirectionalKeyPressedEvent = [&](int8_t DirX, int8_t DirY)
	{
		TextureManager* TextureManager = TextureManagerPtr.get();
		if (!TextureManager)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_MANAGER_PTR INVALID IN KEY_PRESS_EVENT!");
			return;
		}

		SDLManager* SDLManager = SDLManagerPtr.get();
		if (!SDLManager)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN KEY_PRESS_EVENT!");
			return;
		}

		GameInstancePtr->PollKeyEvent(TextureManager, SDLManager, DirX, DirY);
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
	if (!GameInstancePtr || !InputManagerPtr)
	{
		return;
	}

	InputManagerPtr->QuitGameEvent = nullptr;
	InputManagerPtr->DirectionalKeyPressedEvent = nullptr;
	InputManagerPtr->DelSpaceKeyPressedEvent = nullptr;
	GameInstancePtr->SetWindowEvent = nullptr;
}