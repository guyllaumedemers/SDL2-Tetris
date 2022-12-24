#include "../include/SDLManager.h"

void SDLManager::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 INIT FAILED!");
		exit(EXIT_FAILURE);
	}

	SDLWindowUniquePtr = std::unique_ptr<SDL_Window, FreeSDLWindow>(SDL_CreateWindow("TETRIS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		NULL, NULL,
		NULL
	));

	if (!SDLWindowUniquePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 WINDOW CREATION FAILED!");
		exit(EXIT_FAILURE);
	}

	SDLRendererUniquePtr = std::unique_ptr<SDL_Renderer, FreeSDLRenderer>(SDL_CreateRenderer(
		SDLWindowUniquePtr.get(),
		-1,
		SDL_RENDERER_ACCELERATED
	));

	if (!SDLRendererUniquePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 RENDERER CREATION FAILED!");
		exit(EXIT_FAILURE);
	}

	Clock.Initialize();
}

void SDLManager::Update(TextureManager* const TextureManagerPtrArg, std::function<void(TextureManager* const, SDLManager* const)> UpdateFncPtrArg)
{
	if (!SDLRendererUniquePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2 RENDERER UPDATE FAILED!");
		return;
	}

	if (!TextureManagerPtrArg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_MANAGER INVALID!");
		return;
	}

	SDL_Renderer* const SDLRendererPtr = SDLRendererUniquePtr.get();
	static constexpr uint8_t&& Alpha = 255;

	SDL_SetRenderDrawColor(SDLRendererPtr, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(SDLRendererPtr);
	UpdateFncPtrArg(TextureManagerPtrArg, this);
	SDL_RenderPresent(SDLRendererPtr);
}

void SDLManager::Quit() const
{
	// quit sdl context, no need to call explicitly sdl_window_deleter & sdl_renderer_deleter
	SDL_Quit();
}

void SDLManager::SetWindowContextSize(uint16_t Width, uint16_t Height) const
{
	if (!SDLWindowUniquePtr)
	{
		return;
	}

	SDL_SetWindowSize(SDLWindowUniquePtr.get(), Width, Height);
}

void SDLManager::CreateTimeStamp()
{
	Clock.Now = static_cast<double>(SDL_GetTicks64());
}

void SDLManager::LimitFrameRate(float Fps, double Time)
{
	static const double&& MSPerFrame = static_cast<double>((1.f / Fps));
	while (Time < MSPerFrame)
	{
		Time += GetDeltaTime();
	}
}

double SDLManager::GetDeltaTime()
{
	static constexpr double&& MSPerSecond = static_cast<double>(1000.f);

	Clock.Last = Clock.Now;
	Clock.Now = static_cast<double>(SDL_GetTicks64());
	Clock.DeltaTime = (Clock.Now - Clock.Last) / MSPerSecond;
	return Clock.DeltaTime;
}