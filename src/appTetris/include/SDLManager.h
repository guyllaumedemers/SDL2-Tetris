#ifndef INCLUDED_SDL_MANAGER
#define INCLUDED_SDL_MANAGER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#include "SDLWrapper.h"

#ifndef INCLUDED_SDL_CLOCK
#define INCLUDED_SDL_CLOCK
struct SDLClock final
{
	double Last = 0;
	double Now = 0;
	double DeltaTime = 0;

	inline void Initialize() { Now = Last = static_cast<double>(SDL_GetTicks64()); }
};
#endif

#ifndef INCLUDED_SDL_TIMER
#define INCLUDED_SDL_TIMER
struct SDLTimer final
{
	inline void Start(Uint32 CallbackDelay, SDL_TimerCallback CallbackEvent, void* Params)
	{
		(CallbackEvent != nullptr) ? SDL_AddTimer(CallbackDelay, CallbackEvent, Params) : NULL;
	};
};
#endif

class SDLManager final
{
	std::unique_ptr<SDL_Renderer, FreeSDLRenderer> SDLRendererUniquePtr = nullptr;
	std::unique_ptr<SDL_Window, FreeSDLWindow> SDLWindowUniquePtr = nullptr;
	SDLClock Clock;
	SDLTimer Timer;
public:
	SDLManager(const SDLManager&) = delete;
	SDLManager(SDLManager&&) = delete;
	SDLManager() = default;
	~SDLManager() = default;
	SDLManager& operator=(const SDLManager&) = delete;
	SDLManager& operator=(SDLManager&&) = delete;
	void Initialize();
	void Update(class TextureManager* const TextureManagerPtr, std::function<void(class TextureManager* const, SDLManager* const)> UpdateFncPtrArg);
	void Quit() const;
	void SetWindowContextSize(uint16_t Width, uint16_t Height) const;
	void CreateTimeStamp();
	void LimitFrameRate(double Fps, double Time);
	double GetDeltaTime();
	// --- Getter/Setter
	const std::unique_ptr<SDL_Renderer, FreeSDLRenderer>& GetRenderer() const { return SDLRendererUniquePtr; }
	const std::unique_ptr<SDL_Window, FreeSDLWindow>& GetWindow() const { return SDLWindowUniquePtr; }
	SDLTimer& GetTimer() { return Timer; }
	// ---
};
#endif