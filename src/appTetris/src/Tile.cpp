#include "../include/Tile.h"
#include "../include/TextureManager.h"
#include "../include/SDLManager.h"
#include "../include/SDLlogHelper.h"

#ifndef SDL_LIB
#define SDL_LIB
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
		return;
	}

	SDL_Texture* const SDLTextureTargetPtr = SDL_CreateTexture(SDLRendererPtr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Tile::Size, Tile::Size);
	if (!SDLTextureTargetPtr)
	{
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

	static constexpr uint8_t&& Alpha = UINT8_MAX;
	static const std::unordered_map<TileAttributeEnum, std::string>&& TexturePair =
	{
		std::make_pair(TileAttributeEnum::Empty, std::string("Black")),
		std::make_pair(TileAttributeEnum::Border, std::string("Grey")),
		std::make_pair(TileAttributeEnum::Filled, std::string("Wildcard"))
	};

	SDL_SetRenderDrawColor(SDLRendererPtr, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(SDLRendererPtr);

	const auto TexturePairFound = TexturePair.find(Attribute);
	if (TexturePairFound == TexturePair.end())
	{
		SDL_DestroyTexture(SDLTextureTargetPtr);
		SDL_SetRenderTarget(SDLRendererPtr, NULL);
		return;
	}

	SDL_Texture* const SDLTextureTilePtr = TextureManagerPtrArg->GetTextureByName(TexturePairFound->second._Equal(
		std::string("Wildcard"))
		? Wildcard
		: TexturePairFound->second
	);

	if (!SDLTextureTilePtr)
	{
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
