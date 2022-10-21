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
	std::unique_ptr<SDL_Window, FreeSDLWindow> Window = nullptr;
	std::unique_ptr<SDL_Renderer, FreeSDLRenderer> Renderer = nullptr;

public:
	SDLManager() = default;
	~SDLManager() = default;
	void Init();
	void Update(std::function<void()> GameInstanceFuncPtr);
	void Clear();
	void SetWindowContextSize(uint16_t Width, uint16_t Height);
};
#endif