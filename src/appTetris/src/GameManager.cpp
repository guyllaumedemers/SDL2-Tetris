#include "../include/GameManager.h"

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
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
	// initialize game loop
	GameInstanceUniquePtr->Initialize(SDLManagerUniquePtr.get());
}

void GameManager::Update()
{
	bIsQuittingGame = false;
	while (!bIsQuittingGame)
	{
		if (!SDLManagerUniquePtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN GAME_MANAGER UPDATE!");
			exit(EXIT_FAILURE);
		}

		SDL_Event Event;
		if (SDL_PollEvent(&Event) > NULL)
		{
			if (!InputManagerUniquePtr)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: INPUT_MANAGER_PTR INVALID IN GAME_MANAGER UPDATE!");
				exit(EXIT_FAILURE);
			}

			InputManagerUniquePtr->RunEvent(Event);
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

				if (!GameInstanceUniquePtr)
				{
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: GAMEINSTANCE_PTR INVALID IN SDL_MANAGER_UPDATE!");
					return;
				}

				GameInstanceUniquePtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
			});

		SDLManagerUniquePtr->LimitFrameRate(FRAME_RATE);
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
	TextureManagerUniquePtr->Clear();
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
		if (!GameInstanceUniquePtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: GAME_INSTANCE_PTR INVALID IN KEY_PRESS_EVENT!");
			return;
		}

		GameInstanceUniquePtr->PollKeyEvent(DirX, DirY);
	};

	InputManagerUniquePtr->DelSpaceKeyPressedEvent = [&]()
	{
		TetrominoeManager* const TetrominoeManagerPtr = GameInstanceUniquePtr->GetTetrominoeManager().get();
		TileMap* const TileMapPtr = GameInstanceUniquePtr->GetTileMap().get();

		if (!TetrominoeManagerPtr || !TileMapPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOEMANAGER_PTR || TILEMAP_PTR ARE INVALID IN SPACE_PRESS_EVENT!");
			return;
		}

		TetrominoeManagerPtr->Flip(TileMapPtr->GetRows(), TileMapPtr->GetCols());
	};

	GameInstanceUniquePtr->SetWindowEvent = [&](uint16_t Width, uint16_t Height)
	{
		if (!SDLManagerUniquePtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDLMANAGER_PTR IS INVALID IN SETWINDOW CALLBACK!");
			return;
		}

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