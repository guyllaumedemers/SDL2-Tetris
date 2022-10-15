#include "../include/SDLHandle.h"

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 600

int SDLHandle::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 INIT FAILED!");
		return EXIT_FAILURE;
	}

	Window = std::unique_ptr<SDL_Window, FreeSDLWindow>(SDL_CreateWindow("TETRIS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL
	));

	if (!Window)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 WINDOW CREATION FAILED!");
		return EXIT_FAILURE;
	}

	Renderer = std::unique_ptr<SDL_Renderer, FreeSDLRenderer>(SDL_CreateRenderer(
		Window.get(),
		-1,
		SDL_RENDERER_ACCELERATED
	));

	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 RENDERER CREATION FAILED!");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void SDLHandle::Update()
{
	if(!Renderer)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 RENDERER UPDATE FAILED!");
		return;
	}


}

void SDLHandle::Clear()
{
	SDL_Quit();
}