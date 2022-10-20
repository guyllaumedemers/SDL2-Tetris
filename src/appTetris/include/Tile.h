#ifndef INCLUDED_POD_TILE
#define INCLUDED_POD_TILE

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

struct Tile final
{
	static constexpr uint8_t Size = 25;
	uint8_t IndexPosition = 0;

	TileEnum Attribute = TileEnum::Empty;

	friend class TileMap;

	Tile(TileEnum TileEnum, uint8_t Index);
	~Tile() = default;

	void Render() const;
};
#endif
