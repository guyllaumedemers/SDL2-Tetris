#ifndef INCLUDED_TETROMINOE_MANAGER
#define INCLUDED_TETROMINOE_MANAGER

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "Tetrominoe.h"

class TetrominoeManager final
{
	std::vector<std::unique_ptr<Tetrominoe>> TetrominoePool = std::vector<std::unique_ptr<Tetrominoe>>();

	friend class TileMap;

public:
	TetrominoeManager() = default;
	~TetrominoeManager() = default;

	void Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);

private:


};
#endif