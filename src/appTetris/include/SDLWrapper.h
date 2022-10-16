#ifndef INCLUDED_SDL_WRAPPER
#define INCLUDED_SDL_WRAPPER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

struct FreeSDLWindow final
{
	inline void operator()(SDL_Window* Window) const
	{
		if(!Window)
		{
			return;
		}

		SDL_DestroyWindow(Window);
	}
};

struct FreeSDLRenderer
{
	inline void operator()(SDL_Renderer* Renderer) const
	{
		if (!Renderer)
		{
			return;
		}

		SDL_DestroyRenderer(Renderer);
	}
};

#endif
