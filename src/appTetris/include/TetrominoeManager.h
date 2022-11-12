#ifndef INCLUDED_TETROMINOE_MANAGER
#define INCLUDED_TETROMINOE_MANAGER

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

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
	typedef std::function<void(Tetrominoe*)> DelGenerateRandomTetrominoe;
	DelGenerateRandomTetrominoe GenerateRandomTetrominoeEvent;

	std::vector<std::unique_ptr<Tetrominoe>> TetrominoePool = std::vector<std::unique_ptr<Tetrominoe>>();
	std::shared_ptr<Tetrominoe> ActiveTetrominoe = nullptr;
public:
	TetrominoeManager(const TetrominoeManager&) = delete;
	TetrominoeManager(TetrominoeManager&&) = delete;
	TetrominoeManager() = default;
	~TetrominoeManager() = default;
	TetrominoeManager& operator=(const TetrominoeManager&) = delete;
	TetrominoeManager& operator=(TetrominoeManager&&) = delete;
	void Add();
	void Remove();
	void Update(class TileMap* const TilemapPtr, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const;
	void Flip() const;
};
#endif
