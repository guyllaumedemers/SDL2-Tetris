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

void Tetrominoe::Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	if (IsLocked())
	{
		return;
	}

	static constexpr uint8_t&& Zero = 0;

	for (const auto& IndexPosition : TetrominoeEntryIndices)
	{
		const bool&& CanMoveCol = ((IndexPosition % Cols) + DirX) >= Zero && ((IndexPosition % Cols) + DirX) < Cols;
		const bool&& CanMoveRow = ((IndexPosition / Cols) + DirY) >= Zero && ((IndexPosition / Cols) + DirY) < Cols;

		if (!CanMoveRow || !CanMoveCol)
		{
			return;
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