#ifndef INCLUDED_TILEMAP
#define INCLUDED_TILEMAP

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

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
	void Update(class TextureManager* const TextureManagerPtr, class SDLManager* const SDLManagerPtr) const;
	void Clear();
	bool CheckRowCompletion(uint16_t TetrominoeEntryIndex);
	// --- Getter/Setter
	const std::vector<Tile>& GetTiles() const { return Tiles; }
	const uint8_t& GetRows() const { return sRows; }
	const uint8_t& GetCols() const { return sCols; }
	// ---
};
#endif
