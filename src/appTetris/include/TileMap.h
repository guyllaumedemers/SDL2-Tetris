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
	static uint8_t Rows;
	static uint8_t Cols;
	std::vector<Tile> Tilemap = std::vector<Tile>();
public:
	TileMap(const TileMap&) = delete;
	TileMap(TileMap&&) = delete;
	TileMap() = default;
	~TileMap() = default;
	TileMap& operator=(const TileMap&) = delete;
	TileMap& operator=(TileMap&&) = delete;
	void Init(uint8_t Rows, uint8_t Cols, std::function<void(uint16_t, uint16_t)> SetWindowCallback);
	void Update(class TetrominoeManager* const TetrominoeManagerPtr, int8_t DirX, int8_t DirY);
	void Clear();
};
#endif
