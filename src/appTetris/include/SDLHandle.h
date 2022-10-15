#ifndef INCLUDED_SDL_HANDLE
#define INCLUDED_SDL_HANDLE

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "SDLWrapper.h"

class SDLHandle final
{
	std::unique_ptr<SDL_Window, FreeSDLWindow> Window = nullptr;
	std::unique_ptr<SDL_Renderer, FreeSDLRenderer> Renderer = nullptr;

public:

	SDLHandle() = default;
	~SDLHandle() = default;

	int Init();
	void Update();
	void Clear();

private:
};
#endif