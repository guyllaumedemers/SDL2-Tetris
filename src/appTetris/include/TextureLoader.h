#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#ifndef UMAP_H
#define UMAP_H
#include <unordered_map>
#endif

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
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
struct SDL_Renderer;
// ---

#include "SDLWrapper.h"
#include "TraitHelper.h"

class TextureLoader final
{
public:
	TextureLoader(const TextureLoader&) = delete;
	TextureLoader(TextureLoader&&) = delete;
	TextureLoader() = default;
	~TextureLoader() = default;
	TextureLoader& operator=(const TextureLoader&) = delete;
	TextureLoader& operator=(TextureLoader&&) = delete;
	// --- Getter/Setter
	static UnorderedMap<std::string, std::unique_ptr<SDL_Texture, SDLWrapper::FreeSDLTexture>> GetTextures(std::function<SDL_Texture* (const std::string&, SDL_Renderer* const)> FuncPtr, SDL_Renderer* Renderer);
};
#endif