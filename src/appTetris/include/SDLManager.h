#ifndef INCLUDED_SDL_MANAGER
#define INCLUDED_SDL_MANAGER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "SDLWrapper.h"

class SDLManager final
{
	std::unique_ptr<SDL_Window, FreeSDLWindow> Window = nullptr;
	std::unique_ptr<SDL_Renderer, FreeSDLRenderer> Renderer = nullptr;

public:
	SDLManager() = default;
	~SDLManager() = default;
	int Init();
	void Update();
	void Clear();
};
#endif