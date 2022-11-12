#include "../include/Tetrominoe.h"

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
	bool&& IsMoveInBound = true;

	if (IsLocked())
	{
		return !IsMoveInBound;
	}

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
	bool&& IsMoveOverlappingExistingTile = true;

	if (Tilemap.empty())
	{
		return !IsMoveOverlappingExistingTile;
	}

	for(auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		
	}

	return IsMoveOverlappingExistingTile;
}

void Tetrominoe::Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{

}

void Tetrominoe::Flip()
{
	if (IsLocked())
	{
		return;
	}

	// run flip logic, clockwise
}