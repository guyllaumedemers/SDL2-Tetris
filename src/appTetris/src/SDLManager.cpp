#include "../include/SDLManager.h"

void SDLManager::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 INIT FAILED!");
		exit(EXIT_FAILURE);
	}

	Window = std::unique_ptr<SDL_Window, FreeSDLWindow>(SDL_CreateWindow("TETRIS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		NULL, NULL,
		NULL
	));

	if (!Window)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 WINDOW CREATION FAILED!");
		exit(EXIT_FAILURE);
	}

	Renderer = std::unique_ptr<SDL_Renderer, FreeSDLRenderer>(SDL_CreateRenderer(
		Window.get(),
		-1,
		SDL_RENDERER_ACCELERATED
	));

	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 RENDERER CREATION FAILED!");
		exit(EXIT_FAILURE);
	}
}

void SDLManager::Update(std::function<void()> GameInstanceFuncPtr)
{
	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 RENDERER UPDATE FAILED!");
		return;
	}

	SDL_Renderer* Ren = Renderer.get();
	static constexpr uint8_t Alpha = 255;

	SDL_SetRenderDrawColor(Ren, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(Ren);
	/*Render Game Logic before Render Buffer Swap*/
	GameInstanceFuncPtr();
	SDL_RenderPresent(Ren);
}

void SDLManager::Clear()
{
	SDL_Quit();
}

void SDLManager::SetWindowContextSize(uint16_t Width, uint16_t Height)
{
	if (!Window)
	{
		return;
	}
	SDL_SetWindowSize(Window.get(), Width, Height);
}