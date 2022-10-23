#include "../include/Tile.h"
#include "../include/TextureManager.h"
#include "../include/SDLManager.h"

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

Tile::Tile(TileEnum TileEnum, uint8_t Index)
{
	IndexPosition = Index;
	Attribute = TileEnum;
}

void Tile::Render(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, uint8_t Rows, uint8_t Cols)
{
	if (!TextureManagerPtr || SDLManagerPtr)
	{
		return;
	}

	SDL_Renderer* const Ren = SDLManagerPtr->GetRenderer();
	if (!Ren)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_RENDERER_PTR INVALID!");
		return;
	}

	SDL_Texture* const TargetTexture = SDL_CreateTexture(Ren, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_TARGET, Tile::Size, Tile::Size);
	if (!TargetTexture)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_TEXTURE_TARGET_PTR INVALID!");
		return;
	}

	SDL_Rect Rect{
		(IndexPosition % Cols) * Tile::Size,
		(IndexPosition / Rows) * Tile::Size,
		Tile::Size,
		Tile::Size
	};

	// set rendering target to individual tile
	SDL_SetRenderTarget(Ren, TargetTexture);

	static constexpr uint8_t Alpha = 255;
	static const std::unordered_map<TileEnum, std::string> TexturePair =
	{
		std::make_pair(TileEnum::Empty, std::string("Black")),
		std::make_pair(TileEnum::Border, std::string("Grey")),
		std::make_pair(TileEnum::Filled, std::string("Wildcard"))
	};

	SDL_SetRenderDrawColor(Ren, NULL, NULL, NULL, Alpha);
	SDL_RenderClear(Ren);

	auto const Search = TexturePair.find(Attribute);
	if (Search == TexturePair.end())
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: ITERATOR_SEARCH INVALID!");
		SDL_SetRenderTarget(Ren, NULL);
		return;
	}

	std::string const STextureTile = (Search->first == TileEnum::Filled) ? "" : Search->second; // temp - will have to look for tetrominoe type which occupy the space
	if (STextureTile.empty())
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_TEXTURE_TILE_STRING INVALID!");
		SDL_SetRenderTarget(Ren, NULL);
		return;
	}

	SDL_Texture* const TileTexture = TextureManagerPtr->GetTextureByName(STextureTile);
	if (!TileTexture)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL_TEXTURE_TILE_PTR INVALID!");
		SDL_SetRenderTarget(Ren, NULL);
		return;
	}

	SDL_RenderCopy(Ren, TileTexture, &Rect, NULL);

	// set rendering target back to window
	SDL_SetRenderTarget(Ren, NULL);
}