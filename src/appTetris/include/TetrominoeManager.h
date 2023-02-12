#ifndef TETROMINOEMANAGER_H
#define TETROMINOEMANAGER_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef MUTEX_H
#define MUTEX_H
#include <mutex>
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
class Tetrominoe;
// ---

#include "Tile.h"

class TetrominoeManager final
{
	std::vector<std::shared_ptr<Tetrominoe>> TetrominoePool = std::vector<std::shared_ptr<Tetrominoe>>();
	std::shared_ptr<Tetrominoe> ActiveTetrominoe = nullptr;
	std::mutex ActiveTetrominoeMutex;
public:
	TetrominoeManager(const TetrominoeManager&) = delete;
	TetrominoeManager(TetrominoeManager&&) = delete;
	TetrominoeManager() = default;
	~TetrominoeManager() = default;
	TetrominoeManager& operator=(const TetrominoeManager&) = delete;
	TetrominoeManager& operator=(TetrominoeManager&&) = delete;
	void Update(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);
	void Flip(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	void Clear();
	// Utils
	std::shared_ptr<Tetrominoe> GenerateRandomTetromioe(uint8_t Rows, uint8_t Cols) const;
	void Add(std::shared_ptr<Tetrominoe> Tetrominoe);
	void Remove(std::shared_ptr<Tetrominoe> Tetrominoe);
private:
	void Subscribe();
	void UnSubscribe();
public:
};
#endif
