#include "../include/SDLManager.h"

void SDLManager::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 INIT FAILED!");
		exit(EXIT_FAILURE);
	}

	Window = std::unique_ptr<SDL_Window, FreeSDLWindow>(SDL_CreateWindow("TETRIS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		NULL, NULL,
		NULL
	));

	if (!Window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 WINDOW CREATION FAILED!");
		exit(EXIT_FAILURE);
	}

	Renderer = std::unique_ptr<SDL_Renderer, FreeSDLRenderer>(SDL_CreateRenderer(
		Window.get(),
		-1,
		SDL_RENDERER_ACCELERATED
	));

	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 RENDERER CREATION FAILED!");
		exit(EXIT_FAILURE);
	}
}

void SDLManager::Update(TextureManager* const TextureManagerPtr, std::function<void(TextureManager* const, SDLManager* const)> GameInstanceFuncPtr)
{
	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 RENDERER UPDATE FAILED!");
		return;
	}

	if (!TextureManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_MANAGER INVALID!");
		return;
	}

	SDL_Renderer* const Ren = Renderer.get();
	static constexpr uint8_t&& Alpha = 255;

	SDL_SetRenderDrawColor(Ren, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(Ren);
	GameInstanceFuncPtr(TextureManagerPtr, this);
	SDL_RenderPresent(Ren);
}

void SDLManager::Clear() const
{
	SDL_Quit();
}

void SDLManager::SetWindowContextSize(uint16_t Width, uint16_t Height) const
{
	if (!Window)
	{
		return;
	}

	SDL_SetWindowSize(Window.get(), Width, Height);
}
