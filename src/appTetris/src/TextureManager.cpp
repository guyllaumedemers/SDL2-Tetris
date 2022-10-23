#include "../include/TextureManager.h"
#include "../include/TextureLoader.h"
#include "../include/SDLManager.h"

#ifndef INCLUDED_SDL_IMAGE
#define INCLUDED_SDL_IMAGE
#include <SDL_image.h>
#endif

#ifndef INCLUDED_CSTD_LIB
#define INCLUDED_CSTD_LIB
#include <cstdlib>
#endif

void TextureManager::Init(SDLManager* const SDLManagerPtr)
{
	if (!SDLManagerPtr)
	{
		return;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0 /*0 is failure here*/)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2_Image INIT FAILED!");
		exit(EXIT_FAILURE);
	}

	Textures = TextureLoader::GetTextures([&](std::string SFilePath, SDL_Renderer* const InRenderer)
		{
			return IMG_LoadTexture(InRenderer, SFilePath.c_str());
		}, SDLManagerPtr->Renderer.get());
}

void TextureManager::Clear()
{
	IMG_Quit();
	Textures.clear();
}

SDL_Texture* TextureManager::GetTextureByName(std::string TextureName)
{
	if (Textures.empty())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURES EMPTY!");
		return nullptr;
	}

	auto const Search = Textures.find(TextureName);
	return Search != Textures.end() ? Search->second.get() : nullptr;
}