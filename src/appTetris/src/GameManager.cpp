#include "../include/GameManager.h"

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

int GameManager::Run()
{
	Initialize();
	Update();
	Quit();
	return EXIT_SUCCESS;
}

void GameManager::Initialize()
{
	if (!SDLManagerUniquePtr || !InputManagerUniquePtr || !GameInstanceUniquePtr)
	{
		return;
	}
	// subscribe to events
	Subscribe();
	// initialize context
	SDLManagerUniquePtr->Initialize();
	// initialize textures
	TextureManagerUniquePtr->Initialize(SDLManagerUniquePtr.get());
	// run game loop
	GameInstanceUniquePtr->Initialize();
}

void GameManager::Update()
{
	if (!SDLManagerUniquePtr || !InputManagerUniquePtr || !GameInstanceUniquePtr)
	{
		return;
	}

	bIsQuittingGame = false;
	while (!bIsQuittingGame)
	{
		SDL_Event Event;
		if (SDL_WaitEvent(&Event) > NULL)
		{
			InputManagerUniquePtr->WaitPollEvent(Event);
		}

		SDLManagerUniquePtr->Update(
			TextureManagerUniquePtr.get(),
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

				GameInstanceUniquePtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
			});
	}
}

void GameManager::Quit() const
{
	if (!SDLManagerUniquePtr || !TextureManagerUniquePtr || !GameInstanceUniquePtr)
	{
		return;
	}
	// unsubscribe to events
	UnSubscribe();
	// exit game
	GameInstanceUniquePtr->Clear();
	// flush textures
	TextureManagerUniquePtr->Flush();
	// clear context
	SDLManagerUniquePtr->Quit();
}

void GameManager::Subscribe()
{
	if (!GameInstanceUniquePtr || !InputManagerUniquePtr)
	{
		return;
	}

	InputManagerUniquePtr->QuitGameEvent = [&](bool bHasQuitGame)
	{
		bIsQuittingGame = bHasQuitGame;
	};

	InputManagerUniquePtr->DirectionalKeyPressedEvent = [&](int8_t DirX, int8_t DirY)
	{
		TextureManager* const TextureManagerPtr = TextureManagerUniquePtr.get();
		if (!TextureManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_MANAGER_PTR INVALID IN KEY_PRESS_EVENT!");
			return;
		}

		SDLManager* const SDLManagerPtr = SDLManagerUniquePtr.get();
		if (!SDLManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN KEY_PRESS_EVENT!");
			return;
		}

		GameInstanceUniquePtr->PollKeyEvent(TextureManagerPtr, SDLManagerPtr, DirX, DirY);
	};

	InputManagerUniquePtr->DelSpaceKeyPressedEvent = [&]()
	{
		TetrominoeManager* const TetrominoeManagerPtr = GameInstanceUniquePtr->GetTetrominoeManager().get();
		if (!TetrominoeManagerPtr)
		{
			return;
		}
		TetrominoeManagerPtr->Flip();
	};

	GameInstanceUniquePtr->SetWindowEvent = [&](uint16_t Width, uint16_t Height)
	{
		SDLManagerUniquePtr->SetWindowContextSize(Width, Height);
	};
}

void GameManager::UnSubscribe() const
{
	if (!GameInstanceUniquePtr || !InputManagerUniquePtr)
	{
		return;
	}

	InputManagerUniquePtr->QuitGameEvent = nullptr;
	InputManagerUniquePtr->DirectionalKeyPressedEvent = nullptr;
	InputManagerUniquePtr->DelSpaceKeyPressedEvent = nullptr;
	GameInstanceUniquePtr->SetWindowEvent = nullptr;
}