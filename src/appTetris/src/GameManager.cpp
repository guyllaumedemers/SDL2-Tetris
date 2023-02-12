#include "../include/GameManager.h"
#include "../include/GameInstance.h"
#include "../include/InputManager.h"
#include "../include/TextureManager.h"
#include "../include/SDLManager.h"
#include "../include/SDLlogHelper.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

#ifndef FRAME_RATE
#define FRAME_RATE 60.0
#endif

GameManager::GameManager()
{
	GameInstanceSharedPtr = std::make_shared<GameInstance>();
	InputManagerSharedPtr = std::make_shared<InputManager>();
	TextureManagerSharedPtr = std::make_shared<TextureManager>();
	SDLManagerSharedPtr = std::make_shared<SDLManager>();
}

GameManager::~GameManager()
{
	GameInstanceSharedPtr = nullptr;
	InputManagerSharedPtr = nullptr;
	TextureManagerSharedPtr = nullptr;
	SDLManagerSharedPtr = nullptr;
}

int GameManager::Run()
{
	SDLlogHelper::Print(PrefixErrorType::InvalidPtr, std::string("Test"));
	Initialize();
	Update();
	Quit();
	return EXIT_SUCCESS;
}

void GameManager::Initialize()
{
	if (!SDLManagerSharedPtr || !InputManagerSharedPtr || !GameInstanceSharedPtr)
	{
		return;
	}
	// subscribe to events
	Subscribe();
	// initialize context
	SDLManagerSharedPtr->Initialize();
	// initialize textures
	TextureManagerSharedPtr->Initialize(SDLManagerSharedPtr.get());
	// initialize game loop
	GameInstanceSharedPtr->Initialize();
}

void GameManager::Update()
{
	bIsQuittingGame = false;
	while (!bIsQuittingGame)
	{
		if (!SDLManagerSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtr, std::string("GameManager"));
			exit(EXIT_FAILURE);
		}

		SDLManagerSharedPtr->CreateTimeStamp();

		SDL_Event Event;
		if (SDL_PollEvent(&Event) > NULL)
		{
			if (!InputManagerSharedPtr)
			{
				SDLlogHelper::Print(PrefixErrorType::InvalidPtr, std::string("GameManager"));
				exit(EXIT_FAILURE);
			}

			InputManagerSharedPtr->RunEvent(&Event);
		}

		SDLManagerSharedPtr->Update(
			TextureManagerSharedPtr.get(),
			[&](TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg)
			{
				if (!TextureManagerPtrArg)
				{
					SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
					exit(EXIT_FAILURE);
				}

		if (!SDLManagerPtrArg)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			exit(EXIT_FAILURE);
		}

		if (!GameInstanceSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			exit(EXIT_FAILURE);
		}

		GameInstanceSharedPtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
			});

		//SDLManagerUniquePtr->LimitFrameRate(FRAME_RATE, SDLManagerUniquePtr->GetDeltaTime()); // that shit is weird. check it again
	}
}

void GameManager::Quit() const
{
	if (!SDLManagerSharedPtr || !TextureManagerSharedPtr || !GameInstanceSharedPtr)
	{
		return;
	}
	// unsubscribe to events
	UnSubscribe();
	// exit game
	GameInstanceSharedPtr->Clear();
	// flush textures
	TextureManagerSharedPtr->Clear();
	// clear context
	SDLManagerSharedPtr->Quit();
}

void GameManager::Subscribe()
{
	if (!GameInstanceSharedPtr || !InputManagerSharedPtr)
	{
		return;
	}

	InputManagerSharedPtr->GetQuitGameDel() = [&](bool bHasQuitGame)
	{
		bIsQuittingGame = bHasQuitGame;
	};

	InputManagerSharedPtr->GetDirectionalKeyPressedDel() = [&](int8_t DirX, int8_t DirY)
	{
		if (!GameInstanceSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			return;
		}

		GameInstanceSharedPtr->PollKeyEvent(DirX, DirY);
	};

	InputManagerSharedPtr->GetSpaceKeyPressedDel() = [&]()
	{
		if (!GameInstanceSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			return;
		}

		GameInstanceSharedPtr->PollSpaceKeyEvent();
	};

	GameInstanceSharedPtr->SetWindowDelegate = [&](uint16_t Width, uint16_t Height)
	{
		if (!SDLManagerSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			return;
		}

		SDLManagerSharedPtr->SetWindowContextSize(Width, Height);
	};
}

void GameManager::UnSubscribe() const
{
	// leave empty since the Observer will clear its registries
}