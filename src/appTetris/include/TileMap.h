#ifndef TILEMAP_H
#define TILEMAP_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

// --- forward declaration
class TextureManager;
class SDLManager;
// ---

#include "Tile.h"

class TileMap final
{
	std::vector<Tile> Tiles = std::vector<Tile>();
	static uint8_t sRows;
	static uint8_t sCols;
	void ClearRow(size_t StartIndex);
public:
	TileMap(const TileMap&) = delete;
	TileMap(TileMap&&) = delete;
	TileMap() = default;
	~TileMap() = default;
	TileMap& operator=(const TileMap&) = delete;
	TileMap& operator=(TileMap&&) = delete;
	void Initialize(uint8_t Rows, uint8_t Cols, const std::function<void(uint16_t, uint16_t)>& SetWindowFncPtrArg);
	void Update(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr) const;
	void Clear();
	bool CheckRowCompletion(uint16_t TetrominoeEntryIndex);
public:
	// Getter/Setter
	const std::vector<Tile>& GetTiles() const { return Tiles; }
	const uint8_t& GetRows() const { return sRows; }
	const uint8_t& GetCols() const { return sCols; }
};
#endif
