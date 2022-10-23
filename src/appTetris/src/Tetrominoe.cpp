#include "../include/Tetrominoe.h"

Tetrominoe::Tetrominoe(ShapeEnum TetrominoeEnum)
{
	if(!TetrominoeEnum /*ShapeEnum::None == 0*/)
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
	if (bIsLocked)
	{
		return;
	}


}

void Tetrominoe::Flip()
{
	if(bIsLocked)
	{
		return;
	}

	// run flip logic, clockwise
}