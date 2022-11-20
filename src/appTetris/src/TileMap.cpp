#include "../include/TileMap.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
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

	const uint8_t&& Zero = 0;
	const uint8_t&& One = 1;

	for (size_t Index = 0; Index != static_cast<size_t>(Rows * Cols); ++Index)
	{
		const size_t&& Row = (Index / Cols);
		const size_t&& Col = (Index % Cols);

		if (Row == Zero || Row == Rows - One || Col == Zero || Col == Cols - One)
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

void TileMap::CheckRowCompletion(uint16_t IndexPosition)
{
	const uint8_t&& Row = static_cast<uint8_t>(IndexPosition / sCols);
	const size_t&& StartIndex = static_cast<size_t>(Row * sCols);

	try
	{
		for (size_t Index = StartIndex; Index != (StartIndex + sCols); ++Index)
		{
			const Tile& Tile = Tiles.at(Index);
			if (Tile.Attribute == TileAttributeEnum::Empty)
			{
				return;
			}
		}

		ClearRow(Row);
	}
	catch (const std::out_of_range& e)
	{
		// print message
	}
}

void TileMap::ClearRow(uint8_t Row)
{
	const size_t&& StartIndex = static_cast<size_t>(Row * sCols);

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
		// print message
	}
}