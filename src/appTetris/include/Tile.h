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
	TileAttributeEnum Attribute = TileAttributeEnum::Empty;
	std::string TileWildcard = std::string("Undefined");
	size_t Index = 0;
	static inline const uint8_t& Size = STILE;
public:
	Tile(const TileAttributeEnum& NextAttribute, const std::string& NextWildcard, const size_t& NextIndex);
	Tile(const Tile&) = delete;
	Tile(Tile&&) = default;
	Tile() = default;
	~Tile() = default;
	Tile& operator==(const Tile&) = delete;
	Tile& operator==(Tile&&) = delete;
	void Render(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, uint8_t Rows, uint8_t Cols) const;
public:
	// Getter/Setter
	void SetAttribute(const TileAttributeEnum& NextAttribute) { Attribute = NextAttribute; };
	void SetWildcard(const std::string& NextWildcard) { TileWildcard = NextWildcard; };
	void SetIndex(const size_t& NextIndex) { Index = NextIndex; };
	bool IsEqual(TileAttributeEnum NextAttribute) const { return Attribute == NextAttribute; }
};
#endif
