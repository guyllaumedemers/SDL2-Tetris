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

class SDLManager final
{
	std::unique_ptr<SDL_Window, FreeSDLWindow> SDLWindowUniquePtr = nullptr;
	std::unique_ptr<SDL_Renderer, FreeSDLRenderer> SDLRendererUniquePtr = nullptr;
	friend class TextureManager;
public:
	SDLManager(const SDLManager&) = delete;
	SDLManager(SDLManager&&) = delete;
	SDLManager() = default;
	~SDLManager() = default;
	SDLManager& operator=(const SDLManager&) = delete;
	SDLManager& operator=(SDLManager&&) = delete;
	void Initialize();
	void Update(class TextureManager* const TextureManagerPtr, std::function<void(TextureManager* const, SDLManager* const)> GameInstanceFuncPtr);
	void Quit() const;
	void SetWindowContextSize(uint16_t Width, uint16_t Height) const;
	SDL_Renderer* const GetRenderer() const { return SDLRendererUniquePtr.get(); }
};
#endif