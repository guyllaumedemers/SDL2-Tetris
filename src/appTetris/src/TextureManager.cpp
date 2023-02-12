#include "../include/TextureManager.h"
#include "../include/TextureLoader.h"
#include "../include/SDLManager.h"
#include "../include/SDLlogHelper.h"

#ifndef SDLIMAGE_H
#define SDLIMAGE_H
#include <SDL_image.h>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdlib>
#endif

void TextureManager::Initialize(SDLManager* const SDLManagerPtrArg)
{
	if (!SDLManagerPtrArg)
	{
		return;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == NULL /*See IMG_Init Return Value*/)
	{
		SDLlogHelper::Print(PrefixErrorType::LibraryInitFailed, "TextureManager");
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
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TextureManager", e);
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
	auto const Search = Textures.find(TextureName);
	return Search != Textures.end() ? Search->second.get() : nullptr;
}