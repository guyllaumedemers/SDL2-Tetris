#include "../include/Tile.h"
#include "../include/TextureManager.h"
#include "../include/SDLManager.h"

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

void Tile::Render(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg, uint8_t Rows, uint8_t Cols) const
{
	if (!TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	SDL_Renderer* const SDLRendererPtr = SDLManagerPtrArg->GetRenderer().get();
	if (!SDLRendererPtr)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_RENDERER_PTR INVALID!");
		return;
	}

	SDL_Texture* const SDLTextureTargetPtr = SDL_CreateTexture(SDLRendererPtr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Tile::Size, Tile::Size);
	if (!SDLTextureTargetPtr)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_TEXTURE_TARGET_PTR INVALID!");
		return;
	}

	const SDL_Rect SDLTargetRect =
	{
		static_cast<int>(IndexPosition % Cols) * Tile::Size,
		static_cast<int>(IndexPosition / Cols) * Tile::Size,
		Tile::Size,
		Tile::Size
	};

	// set rendering target to individual tile
	SDL_SetRenderTarget(SDLRendererPtr, SDLTextureTargetPtr);

	static constexpr uint8_t&& Alpha = 255;
	static const std::unordered_map<TileEnum, std::string>&& TexturePair =
	{
		std::make_pair(TileEnum::Empty, std::string("Black")),
		std::make_pair(TileEnum::Border, std::string("Grey")),
		std::make_pair(TileEnum::Filled, std::string("Wildcard"))
	};

	SDL_SetRenderDrawColor(SDLRendererPtr, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(SDLRendererPtr);

	const auto TexturePairFound = TexturePair.find(Attribute);
	if (TexturePairFound == TexturePair.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: ITERATOR_SEARCH INVALID!");
		SDL_DestroyTexture(SDLTextureTargetPtr);
		SDL_SetRenderTarget(SDLRendererPtr, NULL);
		return;
	}

	SDL_Texture* const SDLTextureTilePtr = TextureManagerPtrArg->GetTextureByName(TexturePairFound->second._Equal(std::string("Wildcard")) ? Wildcard : TexturePairFound->second);
	if (!SDLTextureTilePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_TEXTURE_TILE_PTR INVALID!");
		SDL_DestroyTexture(SDLTextureTargetPtr);
		SDL_SetRenderTarget(SDLRendererPtr, NULL);
		return;
	}

	SDL_RenderCopy(SDLRendererPtr, SDLTextureTilePtr, NULL, NULL);

	// set rendering target back to window
	SDL_SetRenderTarget(SDLRendererPtr, NULL);

	// render tile on window renderer target
	SDL_RenderCopy(SDLRendererPtr, SDLTextureTargetPtr, NULL, &SDLTargetRect);
	SDL_DestroyTexture(SDLTextureTargetPtr);
}
