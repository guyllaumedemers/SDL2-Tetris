#ifndef TETROMINOE_ENUM
#define TETROMINOE_ENUM

enum class TetrominoeShapeEnum : uint8_t
{
	None = 0,
	TShape = 1,
	LShape = 2,
	ZShape = 4,
	OShape = 8,
	IShape = 16,
	JShape = 32,
	SShape = 64
};

inline TetrominoeShapeEnum operator|(const TetrominoeShapeEnum& lhs, const TetrominoeShapeEnum& rhs)
{
	return (TetrominoeShapeEnum)((int)lhs | (int)rhs);
}

inline TetrominoeShapeEnum operator&(const TetrominoeShapeEnum& lhs, const TetrominoeShapeEnum& rhs)
{
	return (TetrominoeShapeEnum)((int)lhs & (int)rhs);
}

inline TetrominoeShapeEnum operator~(TetrominoeShapeEnum rhs)
{
	return (TetrominoeShapeEnum)(~(int)rhs);
}

inline TetrominoeShapeEnum& operator&=(TetrominoeShapeEnum& lhs, const TetrominoeShapeEnum& rhs)
{
	return (TetrominoeShapeEnum&)((int&)lhs &= (int)rhs);
}

inline TetrominoeShapeEnum& operator|=(TetrominoeShapeEnum& lhs, const TetrominoeShapeEnum& rhs)
{
	return (TetrominoeShapeEnum&)((int&)lhs |= (int)rhs);
}
#endif