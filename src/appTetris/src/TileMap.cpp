#include "../include/TileMap.h"
#include "../include/Tile.h"
#include "../include/SDLlogHelper.h"

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

void TileMap::Initialize(uint8_t Rows, uint8_t Cols, const std::function<void(uint16_t, uint16_t)>& SetWindowFncPtrArg)
{
	Tiles = std::vector<Tile>();
	SetRows(Rows);
	SetCols(Cols);

	static const uint8_t& Zero = 0;
	static const uint8_t& One = 1;

	for (size_t Index = 0; Index != static_cast<size_t>(Rows * Cols); ++Index)
	{
		const size_t& Row = Index / Cols;
		const size_t& Col = Index % Cols;

		if (Row == Zero || Row == (Rows - One) || Col == Zero || Col == (Cols - One))
		{
			Tiles.emplace_back(Tile(TileAttributeEnum::Border, "Undefined", Index));
		}
		else
		{
			Tiles.emplace_back(Tile(TileAttributeEnum::Empty, "Undefined", Index));
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

	const uint8_t& Rows = GetRows();
	const uint8_t& Cols = GetCols();

	for (const auto& Tile : Tiles)
	{
		Tile.Render(TextureManagerPtrArg, SDLManagerPtrArg, Rows, Cols);
	}
}

void TileMap::Clear()
{
	Tiles.clear();
}

bool TileMap::CheckRowCompletion(uint16_t TetrominoeEntryIndex)
{
	const uint8_t& Cols = GetCols();
	const uint8_t& Row = TetrominoeEntryIndex / Cols;
	const size_t& StartIndex = Row * Cols;
	bool bIsRowComplete = true;

	try
	{
		for (size_t Index = StartIndex; Index != (StartIndex + Cols); ++Index)
		{
			const Tile& Tile = Tiles.at(Index);
			if (Tile.IsEqual(TileAttributeEnum::Empty))
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
	static const TileAttributeEnum& EmptyEnum = TileAttributeEnum::Empty;
	static const std::string& UndefinedString = std::string("Undefined");

	try
	{
		const uint8_t& Cols = GetCols();

		for (size_t Index = StartIndex; Index != (StartIndex + Cols); ++Index)
		{
			Tile& Tile = Tiles.at(Index);
			Tile.SetAttribute(EmptyEnum);
			Tile.SetWildcard(UndefinedString);
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TileMap", e);
	}
}