#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

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
	TetrominoeEnum TetrominoeEnumVal = TetrominoeEnum::None;
	int* TetrominoeShapeIndices = nullptr;
	static constexpr uint8_t Size = 25;

	friend class TileMap;

public:
	Tetrominoe(TetrominoeEnum TetrominoeEnum = TetrominoeEnum::None);
	~Tetrominoe() = default;
};
#endif
