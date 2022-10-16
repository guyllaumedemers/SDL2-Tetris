#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_TETROMINOE_ENUM
#define INCLUDED_TETROMINOE_ENUM
enum TetrominoeEnum
{
	None = 0,
	TShape = 1,
	RShape = 2,
	ZShape = 4,
	OShape = 8,
	IShape = 16
};
#endif

class Tetrominoe
{
	static constexpr uint8_t Size = 25;
	TetrominoeEnum TetrominoeEnumVal;
	int* TetrominoeShapeIndices = nullptr;

public:
	Tetrominoe(TetrominoeEnum TetrominoeEnum);
	virtual ~Tetrominoe() = 0 {}

};
#endif
