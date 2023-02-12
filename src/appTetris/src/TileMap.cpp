#include "../include/TileMap.h"
#include "../include/Tile.h"
#include "../include/SDLlogHelper.h"

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

/// <summary>
/// Static Fields
/// </summary>
/// <param name="Rows"></param>
/// <param name="Cols"></param>
uint8_t TileMap::sRows = 0;
uint8_t TileMap::sCols = 0;
//@end()

void TileMap::Initialize(uint8_t Rows, uint8_t Cols, const std::function<void(uint16_t, uint16_t)>& SetWindowFncPtrArg)
{
	Tiles = std::vector<Tile>();
	sRows = Rows;
	sCols = Cols;

	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;

	for (size_t Index = 0; Index != static_cast<size_t>(Rows * Cols); ++Index)
	{
		const size_t&& Row = (Index / Cols);
		const size_t&& Col = (Index % Cols);

		if (Row == Zero || Row == (Rows - One) || Col == Zero || Col == (Cols - One))
		{
			Tiles.emplace_back(Tile{ TileAttributeEnum::Border, std::string("Undefined"), Index });
		}
		else
		{
			Tiles.emplace_back(Tile{ TileAttributeEnum::Empty, std::string("Undefined"), Index });
		}
	}

	SetWindowFncPtrArg(Cols * Tile::Size, Rows * Tile::Size);
}

void TileMap::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	for (const auto& Tile : Tiles)
	{
		Tile.Render(TextureManagerPtrArg, SDLManagerPtrArg, sRows, sCols);
	}
}

void TileMap::Clear()
{
	Tiles.clear();
}

bool TileMap::CheckRowCompletion(uint16_t TetrominoeEntryIndex)
{
	const uint8_t&& Row = static_cast<uint8_t>(TetrominoeEntryIndex / sCols);
	const size_t&& StartIndex = static_cast<size_t>(Row * sCols);
	bool&& bIsRowComplete = true;

	try
	{
		for (size_t Index = StartIndex; Index != (StartIndex + sCols); ++Index)
		{
			const Tile& Tile = Tiles.at(Index);
			if (Tile.Attribute == TileAttributeEnum::Empty)
			{
				return !bIsRowComplete;
			}
		}

		ClearRow(StartIndex);
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TileMap", e);
		bIsRowComplete = false;
	}
	return bIsRowComplete;
}

void TileMap::ClearRow(size_t StartIndex)
{
	try
	{
		for (size_t Index = StartIndex; Index != (StartIndex + sCols); ++Index)
		{
			Tile& Tile = Tiles.at(Index);

			Tile.Attribute = TileAttributeEnum::Empty;
			Tile.Wildcard = std::string("Undefined");
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TileMap", e);
	}
}