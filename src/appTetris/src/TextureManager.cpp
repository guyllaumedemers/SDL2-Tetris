#include "../include/TextureManager.h"
#include "../include/TextureLoader.h"
#include "../include/SDLManager.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

#ifndef INCLUDED_SDL_IMAGE
#define INCLUDED_SDL_IMAGE
#include <SDL_image.h>
#endif

#ifndef INCLUDED_CSTD_LIB
#define INCLUDED_CSTD_LIB
#include <cstdlib>
#endif

void TextureManager::Initialize(SDLManager* const SDLManagerPtrArg)
{
	if (!SDLManagerPtrArg)
	{
		return;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0 /*0 is failure here*/)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL2_Image INIT FAILED!");
		exit(EXIT_FAILURE);
	}

	Textures = TextureLoader::GetTextures([&](const std::string& SFilePath, SDL_Renderer* const SDLRendererPtrArg)
		{
			SDL_Texture* OutTexture = nullptr;
			try
			{
				OutTexture = IMG_LoadTexture(SDLRendererPtrArg, SFilePath.c_str());
				if (!OutTexture)
				{
					throw std::exception();
				}
				return OutTexture;
			}
			catch (const std::exception& e)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURE_TARGET INVALID! %s", e.what());
			}
			return OutTexture;
		}, SDLManagerPtrArg->GetRenderer().get());
}

void TextureManager::Clear()
{
	Textures.clear();
	IMG_Quit();
}

SDL_Texture* const TextureManager::GetTextureByName(const std::string& TextureName) const
{
	if (Textures.empty())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TEXTURES EMPTY!");
		return nullptr;
	}

	auto const Search = Textures.find(TextureName);
	return Search != Textures.end() ? Search->second.get() : nullptr;
}