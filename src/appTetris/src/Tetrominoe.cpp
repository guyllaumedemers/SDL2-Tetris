#include "../include/Tetrominoe.h"

Tetrominoe::Tetrominoe(ShapeEnum TetrominoeEnum)
{
	if(!TetrominoeEnum /*ShapeEnum::None == 0*/)
	{
		return;
	}
	TetrominoeShape = Shape(TetrominoeEnum);
}