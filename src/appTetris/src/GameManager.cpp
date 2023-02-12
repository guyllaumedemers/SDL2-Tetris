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
		PollEvents();
		Render();
	}
}

void GameManager::PollEvents() const
{
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
}

void GameManager::Render() const
{
	static const std::function<void(TextureManager* const, SDLManager* const)>& RenderingUpdate =
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
	};

	if (!SDLManagerSharedPtr)
	{
		SDLlogHelper::Print(PrefixErrorType::InvalidPtr, std::string("GameManager"));
		exit(EXIT_FAILURE);
	}

	SDLManagerSharedPtr->Update(
		TextureManagerSharedPtr.get(),
		RenderingUpdate
	);
}

void GameManager::Quit() const
{
	if (!SDLManagerSharedPtr || !TextureManagerSharedPtr || !GameInstanceSharedPtr)
	{
		return;
	}
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

	InputManagerSharedPtr->GetRestartGameDel() = [&]()
	{
		if (!GameInstanceSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			return;
		}

		GameInstanceSharedPtr->RestartGame();
	};

	InputManagerSharedPtr->GetQuitGameDel() = [&](bool bHasQuitGame)
	{
		bIsQuittingGame = bHasQuitGame;
	};

	GameInstanceSharedPtr->GetSetWindowDel() = [&](uint16_t Width, uint16_t Height)
	{
		if (!SDLManagerSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, std::string("GameManager"));
			return;
		}

		SDLManagerSharedPtr->SetWindowContextSize(Width, Height);
	};
}