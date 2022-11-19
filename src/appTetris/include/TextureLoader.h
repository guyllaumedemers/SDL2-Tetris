#ifndef INCLUDED_TEXTURE_LOADER
#define INCLUDED_TEXTURE_LOADER

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

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#include "SDLWrapper.h"

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
	static std::unordered_map<std::string, std::unique_ptr<SDL_Texture, FreeSDLTexture>> GetTextures(std::function<SDL_Texture* (const std::string&, SDL_Renderer* const)> FuncPtr, SDL_Renderer* Renderer);
	// ---
};
#endif