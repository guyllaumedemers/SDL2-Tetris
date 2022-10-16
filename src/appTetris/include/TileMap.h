#ifndef INCLUDED_TILEMAP
#define INCLUDED_TILEMAP

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_ARRAY
#define INCLUDED_COLLECTION_ARRAY
#include <vector>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "Tetrominoe.h"

class TileMap final
{
	static uint8_t Rows;
	static uint8_t Cols;
	std::vector<std::unique_ptr<Tetrominoe>> Tilemap = std::vector<std::unique_ptr<Tetrominoe>>();

public:
	TileMap() = default;
	~TileMap() = default;
	void Init(uint8_t Rows, uint8_t Cols, std::function<void(uint16_t, uint16_t)> SetWindowCallback);
	void Update(int8_t DirX, int8_t DirY);
	void Clear();
};
#endif
