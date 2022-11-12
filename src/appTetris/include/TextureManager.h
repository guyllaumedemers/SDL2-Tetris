#ifndef INCLUDED_TEXTURE_MANAGER
#define INCLUDED_TEXTURE_MANAGER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_STRING
#define INCLUDED_STRING
#include <string>
#endif

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "SDLWrapper.h"

class TextureManager
{
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> Textures;
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;
	void Initialize(class SDLManager* const SDLManagerPtr);
	void Flush();
	SDL_Texture* GetTextureByName(const std::string& TextureName) const;
};
#endif