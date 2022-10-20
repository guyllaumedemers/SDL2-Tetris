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

class TetrominoeManager final
{
	typedef std::function<void(class Tetrominoe*)> DelGenerateRandomTetrominoe;
	DelGenerateRandomTetrominoe GenerateRandomTetrominoeEvent;

	std::vector<std::unique_ptr<class Tetrominoe>> TetrominoePool = std::vector<std::unique_ptr<class Tetrominoe>>();
	std::shared_ptr<class Tetrominoe> ActiveTetrominoe = nullptr;

	friend class TileMap;

public:
	TetrominoeManager() = default;
	~TetrominoeManager() = default;

	void Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);
};
#endif