#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#ifndef UMAP_H
#define UMAP_H
#include <unordered_map>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

// --- forward declaration
struct SDL_Texture;
class SDLManager;
// ---

#include "SDLWrapper.h"
#include "TraitHelper.h"

class TextureManager
{
	UnorderedMap<std::string, std::unique_ptr<SDL_Texture, SDLWrapper::FreeSDLTexture>> Textures;
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;
	void Initialize(SDLManager* const SDLManagerPtr);
	void Clear();
public:
	// --- Getter/Setter
	SDL_Texture* const GetTextureByName(const std::string& TextureName) const;
	// --- 
};
#endif