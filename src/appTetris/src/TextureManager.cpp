#include "../include/TextureManager.h"
#include "../include/TextureLoader.h"

void TextureManager::Init(SDL_Renderer* Renderer)
{
	Textures = TextureLoader::GetTextures([&](std::string SFilePath, SDL_Renderer* Renderer)
		{
			// IMG_LOAD
			SDL_Texture* TargetTexture = nullptr;
			return TargetTexture;
		}, Renderer);
}

void TextureManager::Clear()
{
	Textures.clear();
}

SDL_Texture* TextureManager::GetTextureByName(std::string TextureName)
{
	if (Textures.empty())
	{
		return nullptr;
	}
	auto Search = Textures.find(TextureName);
	return Search != Textures.end() ? Search->second.get() : nullptr;
}