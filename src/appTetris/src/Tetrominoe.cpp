#include "../include/Tetrominoe.h"
#include "../include/Tile.h"

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

Tetrominoe::Tetrominoe(ShapeEnum TetrominoeEnum, uint8_t Rows, uint8_t Cols)
{
	if (!TetrominoeEnum /*ShapeEnum::None == 0*/)
	{
		return;
	}

	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;
	static constexpr uint8_t&& Two = 2;
	static constexpr uint8_t&& Three = 3;

	switch (TetrominoeEnum)
	{
	case None:
		TetrominoeEntryIndices = { Zero, Zero, Zero, Zero };
		break;
	case TShape:
		TetrominoeEntryIndices = { Zero, One, Two, static_cast<uint8_t>(One + (One * Cols)) };
		break;
	case LShape:
		TetrominoeEntryIndices = { Zero, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(Two * Cols), static_cast<uint8_t>(One + (Two * Cols)) };
		break;
	case ZShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case OShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	case IShape:
		TetrominoeEntryIndices = { Zero, One, Two, Three };
		break;
	case JShape:
		TetrominoeEntryIndices = { One, static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two * Cols), static_cast<uint8_t>(One + (Two * Cols)) };
		break;
	case SShape:
		TetrominoeEntryIndices = { One, Two, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	default:
		break;
	}
	Pattern = TetrominoeEnum;
}

bool Tetrominoe::IsMoveInBound(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static constexpr bool&& IsMoveInBound = true;
	static constexpr uint8_t&& Zero = 0;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const bool&& CanMoveCol = ((TetrominoeEntryIndex % Cols) + DirX) >= Zero && ((TetrominoeEntryIndex % Cols) + DirX) < Cols;
		const bool&& CanMoveRow = ((TetrominoeEntryIndex / Cols) + DirY) >= Zero && ((TetrominoeEntryIndex / Cols) + DirY) < Cols;

		if (!CanMoveRow || !CanMoveCol)
		{
			return !IsMoveInBound;
		}
	}

	return IsMoveInBound;
}

bool Tetrominoe::IsMoveOverlappingExistingTile(const std::vector<Tile>& Tilemap, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static constexpr bool&& IsMoveOverlappingExistingTile = true;

	if (Tilemap.empty())
	{
		return !IsMoveOverlappingExistingTile;
	}

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const uint8_t&& JumpValue = static_cast<uint8_t>(DirX + (std::abs(DirY) * Cols));
		const Tile& Tile = Tilemap.at(TetrominoeEntryIndex + JumpValue);

		if (Tile.Attribute == TileEnum::Filled)
		{
			return IsMoveOverlappingExistingTile;
		}
	}

	return !IsMoveOverlappingExistingTile;
}

void Tetrominoe::Update(std::vector<Tile>& Tilemap, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	if (Tilemap.empty())
	{
		return;
	}

	static const std::unordered_map<ShapeEnum, std::string>&& ShapePair =
	{
		std::make_pair(ShapeEnum::None, std::string("Undefined")),
		std::make_pair(ShapeEnum::TShape, std::string("Purple")),
		std::make_pair(ShapeEnum::LShape, std::string("Orange")),
		std::make_pair(ShapeEnum::ZShape, std::string("Red")),
		std::make_pair(ShapeEnum::OShape, std::string("Yellow")),
		std::make_pair(ShapeEnum::IShape, std::string("Cyan")),
		std::make_pair(ShapeEnum::JShape, std::string("Blue")),
		std::make_pair(ShapeEnum::SShape, std::string("Green"))
	};

	const auto ShapePairFound = ShapePair.find(Pattern);
	if (ShapePairFound == ShapePair.end())
	{
		return;
	}

	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		{
			Tile& PreviousTile = Tilemap.at(TetrominoeEntryIndex);

			PreviousTile.Attribute = TileEnum::Empty;
			PreviousTile.Wildcard = std::string("Undefined");
		}

		const uint8_t&& JumpValue = static_cast<uint8_t>(DirX + (std::abs(DirY) * Cols));
		TetrominoeEntryIndex += JumpValue;

		{
			Tile& NextTile = Tilemap.at(TetrominoeEntryIndex);

			NextTile.Attribute = TileEnum::Filled;
			NextTile.Wildcard = ShapePairFound->second;
		}
	}
}

void Tetrominoe::Flip()
{
	if (IsLocked())
	{
		return;
	}

	// run flip logic, clockwise
}