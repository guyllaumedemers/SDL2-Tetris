#include "../include/Tile.h"
#include "../include/TextureManager.h"
#include "../include/SDLManager.h"

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

Tile::Tile(TileEnum TileEnum, size_t Index)
{
	this->IndexPosition = Index;
	this->Attribute = TileEnum;
}

void Tile::Render(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, uint8_t Rows, uint8_t Cols)
{
	if (!TextureManagerPtr || !SDLManagerPtr)
	{
		return;
	}

	SDL_Renderer* const Renderer = SDLManagerPtr->GetRenderer();
	if (!Renderer)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_RENDERER_PTR INVALID!");
		return;
	}

	SDL_Texture* const TextureTarget = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Tile::Size, Tile::Size);
	if (!TextureTarget)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_TEXTURE_TARGET_PTR INVALID!");
		return;
	}

	SDL_Rect const TextureRect =
	{
		static_cast<int>(IndexPosition % Cols) * Tile::Size,
		static_cast<int>(IndexPosition / Cols) * Tile::Size,
		Tile::Size,
		Tile::Size
	};

	// set rendering target to individual tile
	SDL_SetRenderTarget(Renderer, TextureTarget);

	static constexpr uint8_t Alpha = 255;
	static const std::unordered_map<TileEnum, std::string> TexturePair =
	{
		std::make_pair(TileEnum::Empty, std::string("Black")),
		std::make_pair(TileEnum::Border, std::string("Grey")),
		std::make_pair(TileEnum::Filled, std::string("Wildcard"))
	};

	SDL_SetRenderDrawColor(Renderer, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(Renderer);

	auto const Search = TexturePair.find(Attribute);
	if (Search == TexturePair.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: ITERATOR_SEARCH INVALID!");
		SDL_DestroyTexture(TextureTarget);
		SDL_SetRenderTarget(Renderer, NULL);
		return;
	}

	std::string const STextureTile = (Search->first == TileEnum::Filled) ? "" : Search->second;
	if (STextureTile.empty())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_TEXTURE_TILE_STRING INVALID!");
		SDL_DestroyTexture(TextureTarget);
		SDL_SetRenderTarget(Renderer, NULL);
		return;
	}

	SDL_Texture* const TextureTile = TextureManagerPtr->GetTextureByName(STextureTile);
	if (!TextureTile)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_TEXTURE_TILE_PTR INVALID!");
		SDL_DestroyTexture(TextureTarget);
		SDL_SetRenderTarget(Renderer, NULL);
		return;
	}

	SDL_RenderCopy(Renderer, TextureTile, NULL, NULL);

	// set rendering target back to window
	SDL_SetRenderTarget(Renderer, NULL);

	// render tile on window renderer target
	SDL_RenderCopy(Renderer, TextureTarget, NULL, &TextureRect);
	SDL_DestroyTexture(TextureTarget);
}
