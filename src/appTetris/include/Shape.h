#ifndef INCLUDED_SHAPE
#define INCLUDED_SHAPE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_ARRAY
#define INCLUDED_COLLECTION_ARRAY
#include <array>
#endif

#ifndef INCLUDED_TETROMINOE_ENUM
#define INCLUDED_TETROMINOE_ENUM
enum ShapeEnum
{
	None = 0,
	TShape = 1,
	LShape = 2,
	ZShape = 4,
	OShape = 8,
	IShape = 16
};
#endif

struct Shape final
{
	static constexpr size_t MaxEntriesPerShape = 4;
	ShapeEnum Pattern = ShapeEnum::None;

	std::array<uint8_t, MaxEntriesPerShape> ShapeIndices = std::array<uint8_t, MaxEntriesPerShape>();

	Shape(ShapeEnum ShapeEnum);
	Shape() = default;
	~Shape() = default;

	void Update(int8_t DirX, int8_t DirY);
};
#endif