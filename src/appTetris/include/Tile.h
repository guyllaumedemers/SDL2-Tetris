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
	TileEnum Attribute = TileEnum::Empty;
	static constexpr uint8_t Size = 25;
	uint8_t IndexPosition = 0;
	friend class TileMap;

	//Tile(const Tile&) = delete;	// std::vector use copy constructor for assigning adding new entries
	//Tile(Tile&&) = delete;		// std::vector use std::move for resizing
	Tile() = delete;
	Tile(TileEnum TileEnum, uint8_t Index);
	~Tile() = default;
	//Tile& operator=(const Tile&) = delete;
	//Tile& operator=(Tile&&) = delete;
	void Render() const;
};
#endif
