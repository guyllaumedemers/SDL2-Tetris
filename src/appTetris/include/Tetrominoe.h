#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_COLLECTION_ARRAY
#define INCLUDED_COLLECTION_ARRAY
#include <array>
#endif

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_TILE_ENUM
#define INCLUDED_TILE_ENUM
enum TileEnum
{
	Empty = 0,
	Border = 1,
	Filled = 2
};
#endif

#ifndef INCLUDED_POD_TILE
#define INCLUDED_POD_TILE
struct Tile final
{
	static constexpr uint8_t Size = 25;
	uint8_t IndexPosition = 0;

	TileEnum Attribute = TileEnum::Empty;

	friend class TileMap;

	Tile(TileEnum TileEnum, uint8_t Index);
	~Tile() = default;
};

inline Tile::Tile(TileEnum TileEnum, uint8_t Index)
{
	IndexPosition = Index;
	Attribute = TileEnum;
}
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

#ifndef INCLUDED_SHAPE
#define INCLUDED_SHAPE
struct Shape final
{
	static constexpr size_t MaxEntriesPerShape = 4;
	ShapeEnum Pattern = ShapeEnum::None;

	std::array<uint8_t, MaxEntriesPerShape> ShapeIndices = std::array<uint8_t, MaxEntriesPerShape>();

	Shape(ShapeEnum ShapeEnum);
	Shape() = default;
	~Shape() = default;
};

inline Shape::Shape(ShapeEnum ShapeEnum)
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
#endif

class Tetrominoe final
{
	static constexpr uint8_t SpawnPosition = 15;
	Shape TetrominoeShape;

public:
	Tetrominoe(ShapeEnum TetrominoeEnum = ShapeEnum::None);
	~Tetrominoe() = default;
};
#endif
