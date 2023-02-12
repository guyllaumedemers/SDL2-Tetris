#ifndef TILE_H
#define TILE_H

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

#ifndef STILE
#define STILE 20
#endif

#ifndef TILE_ENUM
#define TILE_ENUM
enum TileAttributeEnum
{
	Empty = 0,
	Border = 1,
	Filled = 2
};
#endif

// --- forward declaration
class TextureManager;
class SDLManager;
// ---

struct Tile final
{
	static constexpr uint8_t&& Size = STILE;
	TileAttributeEnum Attribute = TileAttributeEnum::Empty;
	std::string Wildcard = std::string();
	size_t IndexPosition = 0;
public:
	~Tile() = default;
	Tile& operator==(const Tile&) = delete;
	Tile& operator==(Tile&&) = delete;
	void Render(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, uint8_t Rows, uint8_t Cols) const;
};
#endif
