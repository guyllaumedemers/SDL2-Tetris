#include "../include/SDLTimerManager.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

// --- static fields
std::unique_ptr<SDLTimerManager> SDLTimerManager::Singleton = nullptr;
// ---

SDLTimerManager::SDLTimerID SDLTimerManager::SDLTimer::Start(uint32_t CallbackDelay, uint32_t(*CallbackEvent)(uint32_t, void*), void* Param)
{
	return (CallbackEvent != nullptr)
		? SDL_AddTimer(CallbackDelay, CallbackEvent, Param)
		: NULL;
}

SDLTimerManager::SDLResult SDLTimerManager::SDLTimer::Stop(SDLTimerManager::SDLTimerID TimerID)
{
	return SDL_RemoveTimer(TimerID);
}

int SDLTimerManager::CreateTimer(uint32_t(*Functor)(uint32_t, void*), uint32_t TimerDelay, void* Param)
{
	return Get()->GetTimerHandle().Start(
		TimerDelay,
		Functor,
		Param
	);
}

int SDLTimerManager::DestroyTimer(int TimerID)
{
	return Get()->GetTimerHandle().Stop(
		TimerID
	);
}

SDLTimerManager* SDLTimerManager::Get()
{
	/// <summary>
	/// cannot use make_shared or make_unique with private constructor level
	/// </summary>
	/// <returns></returns>
	if (!Singleton)
	{
		Singleton = std::make_unique<SDLTimerManager>();
	}
	return Singleton.get();
}
