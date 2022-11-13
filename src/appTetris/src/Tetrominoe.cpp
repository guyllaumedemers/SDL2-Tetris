#include "../include/Tetrominoe.h"
#include "../include/Tile.h"

Tetrominoe::Tetrominoe(ShapeEnum TetrominoeEnum)
{
	if (!TetrominoeEnum /*ShapeEnum::None == 0*/)
	{
		return;
	}
	switch (TetrominoeEnum)
	{
	case None:
		break;
	case TShape:
		TetrominoeEntryIndices = {};
		break;
	case LShape:
		TetrominoeEntryIndices = {};
		break;
	case ZShape:
		TetrominoeEntryIndices = {};
		break;
	case OShape:
		TetrominoeEntryIndices = {};
		break;
	case IShape:
		TetrominoeEntryIndices = {};
		break;
	default:
		break;
	}
	Pattern = TetrominoeEnum;
}

bool Tetrominoe::IsMoveInBound(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static constexpr uint8_t&& Zero = 0;
	static constexpr bool&& IsMoveInBound = true;

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
	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		Tile& PreviousTile = Tilemap.at(TetrominoeEntryIndex);
		PreviousTile.Attribute = TileEnum::Empty;

		const uint8_t&& JumpValue = static_cast<uint8_t>(DirX + (std::abs(DirY) * Cols));
		TetrominoeEntryIndex += JumpValue;

		Tile& NextTile = Tilemap.at(TetrominoeEntryIndex);
		NextTile.Attribute = TileEnum::Filled;
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