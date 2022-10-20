#include "../include/Shape.h"

Shape::Shape(ShapeEnum ShapeEnum)
{
	/*Set ShapeIndices with proper index array - Require Cols to handle return*/
	switch (ShapeEnum)
	{
	case None:
		break;
	case TShape:
		ShapeIndices = {};
		break;
	case LShape:
		ShapeIndices = {};
		break;
	case ZShape:
		ShapeIndices = {};
		break;
	case OShape:
		ShapeIndices = {};
		break;
	case IShape:
		ShapeIndices = {};
		break;
	default:
		break;
	}
	Pattern = ShapeEnum;
}
