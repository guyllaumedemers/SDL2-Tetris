#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

// --- forward declaration
struct SDL_Window;
struct SDL_Renderer;
class TextureManager;
// ---

#include "SDLWrapper.h"

class SDLManager final
{
	// aggregated clock handle definition
	struct SDLClock final
	{
		double Last = 0;
		double Now = 0;
		double DeltaTime = 0;

		void Initialize();
	};

	std::unique_ptr<SDL_Window, SDLWrapper::FreeSDLWindow> SDLWindowUniquePtr = nullptr;
	std::unique_ptr<SDL_Renderer, SDLWrapper::FreeSDLRenderer> SDLRendererUniquePtr = nullptr;
	SDLClock Clock;
public:
	SDLManager(const SDLManager&) = delete;
	SDLManager(SDLManager&&) = delete;
	SDLManager() = default;
	~SDLManager() = default;
	SDLManager& operator=(const SDLManager&) = delete;
	SDLManager& operator=(SDLManager&&) = delete;
	void Initialize();
	void Update(TextureManager* const TextureManagerPtr, std::function<void(TextureManager* const, SDLManager* const)> UpdateFncPtrArg);
	void Quit() const;
	void SetWindowContextSize(uint16_t Width, uint16_t Height) const;
	void CreateTimeStamp();
	void LimitFrameRate(double Fps, double Time);
	double GetDeltaTime();
public:
	// Getter/Setter
	const std::unique_ptr<SDL_Window, SDLWrapper::FreeSDLWindow>& GetWindow() const { return SDLWindowUniquePtr; }
	const std::unique_ptr<SDL_Renderer, SDLWrapper::FreeSDLRenderer>& GetRenderer() const { return SDLRendererUniquePtr; }
};
#endif