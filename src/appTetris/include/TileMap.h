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
	static uint8_t sRows;
	static uint8_t sCols;
	std::vector<Tile> Tilemap = std::vector<Tile>();
	friend class TetrominoeManager;
public:
	TileMap(const TileMap&) = delete;
	TileMap(TileMap&&) = delete;
	TileMap() = default;
	~TileMap() = default;
	TileMap& operator=(const TileMap&) = delete;
	TileMap& operator=(TileMap&&) = delete;
	void Initialize(uint8_t Rows, uint8_t Cols, const std::function<void(uint16_t, uint16_t)>& SetWindowFncPtrArg);
	void Update(class TextureManager* const TextureManagerPtr, class TetrominoeManager* const TetrominoeManagerPtr, class SDLManager* const SDLManagerPtr, int8_t DirX, int8_t DirY);
	void Clear();
};
#endif
