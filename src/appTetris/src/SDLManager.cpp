#include "../include/SDLManager.h"
#include "../include/SDLlogHelper.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

void SDLManager::SDLClock::Initialize()
{
	Now = Last = SDL_GetTicks64();
}

void SDLManager::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != NULL /*See SDL_Init Return Value*/)
	{
		SDLlogHelper::Print(PrefixErrorType::LibraryInitFailed, "SDLManager");
		exit(EXIT_FAILURE);
	}

	SDLWindowUniquePtr = std::unique_ptr<SDL_Window, SDLWrapper::FreeSDLWindow>(
		SDL_CreateWindow(
			"TETRIS",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			NULL,
			NULL,
			NULL
		));

	if (!SDLWindowUniquePtr)
	{
		SDLlogHelper::Print(PrefixErrorType::LibraryInitFailed, "SDLManager");
		exit(EXIT_FAILURE);
	}

	SDLRendererUniquePtr = std::unique_ptr<SDL_Renderer, SDLWrapper::FreeSDLRenderer>(
		SDL_CreateRenderer(
			SDLWindowUniquePtr.get(),
			-1,
			SDL_RENDERER_ACCELERATED
		));

	if (!SDLRendererUniquePtr)
	{
		SDLlogHelper::Print(PrefixErrorType::LibraryInitFailed, "SDLManager");
		exit(EXIT_FAILURE);
	}

	Clock.Initialize();
}

void SDLManager::Update(TextureManager* const TextureManagerPtrArg, std::function<void(TextureManager* const, SDLManager* const)> UpdateFncPtrArg)
{
	if (!SDLRendererUniquePtr)
	{
		return;
	}

	if (!TextureManagerPtrArg)
	{
		return;
	}

	SDL_Renderer* const SDLRendererPtr = SDLRendererUniquePtr.get();
	static const uint8_t& Alpha = UINT8_MAX;

	SDL_SetRenderDrawColor(SDLRendererPtr, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(SDLRendererPtr);
	UpdateFncPtrArg(TextureManagerPtrArg, this);
	SDL_RenderPresent(SDLRendererPtr);
}

void SDLManager::Quit() const
{
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
	Clock.Now = SDL_GetTicks64();
}

void SDLManager::LimitFrameRate(double Fps, double Time)
{
	static const double& MSPerFrame = (1.f / Fps);

	while (Time < MSPerFrame)
	{
		Time += GetDeltaTime();
	}
}

double SDLManager::GetDeltaTime()
{
	static const double& MSPerSecond = 1000.f;

	Clock.Last = Clock.Now;
	Clock.Now = SDL_GetTicks64();
	Clock.DeltaTime = (Clock.Now - Clock.Last) / MSPerSecond;
	return Clock.DeltaTime;
}
