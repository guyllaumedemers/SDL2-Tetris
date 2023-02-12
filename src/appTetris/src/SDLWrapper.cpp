#include "../include/SDLWrapper.h"

#ifndef SDL_LIB
#define SDL_LIB
#include <SDL.h>
#endif

void SDLWrapper::FreeSDLWindow::operator()(SDL_Window* Window) const
{
	if (!Window)
	{
		return;
	}

	SDL_DestroyWindow(Window);
}

void SDLWrapper::FreeSDLRenderer::operator()(SDL_Renderer* Renderer) const
{
	if (!Renderer)
	{
		return;
	}

	SDL_DestroyRenderer(Renderer);
}

void SDLWrapper::FreeSDLTexture::operator()(SDL_Texture* Texture) const
{
	if (!Texture)
	{
		return;
	}

	SDL_DestroyTexture(Texture);
}