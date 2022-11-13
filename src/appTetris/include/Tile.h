#ifndef INCLUDED_POD_TILE
#define INCLUDED_POD_TILE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_STRING
#define INCLUDED_STRING
#include <string>
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
	static constexpr uint8_t&& Size = 20;
	TileEnum Attribute = TileEnum::Empty;
	std::string Wildcard = std::string();
	size_t IndexPosition = 0;
	void Render(class TextureManager* const TextureManagerPtr, class SDLManager* const SDLManagerPtr, uint8_t Rows, uint8_t Cols) const;
};
#endif
