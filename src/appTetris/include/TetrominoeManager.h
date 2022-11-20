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
	std::vector<std::shared_ptr<Tetrominoe>> TetrominoePool = std::vector<std::shared_ptr<Tetrominoe>>();
	std::shared_ptr<Tetrominoe> ActiveTetrominoe = nullptr;

	void Add(uint8_t Rows, uint8_t Cols);
	void Remove();
	std::unique_ptr<Tetrominoe> GenerateRandomTetromioeShape(uint8_t Rows, uint8_t Cols) const;
	void RealignTetrominoes(class TileMap* const TileMapPtrArg) const;
public:
	TetrominoeManager(const TetrominoeManager&) = delete;
	TetrominoeManager(TetrominoeManager&&) = delete;
	TetrominoeManager() = default;
	~TetrominoeManager() = default;
	TetrominoeManager& operator=(const TetrominoeManager&) = delete;
	TetrominoeManager& operator=(TetrominoeManager&&) = delete;
	void Initialize(class TileMap* const TileMapPtrArg);
	void Update(class TileMap* const TileMapPtrArg, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const;
	void Clear();
	void Flip(uint8_t Rows, uint8_t Cols) const;
	// --- Getter/Setter
	const std::vector<std::shared_ptr<Tetrominoe>>& GetTetrominoes() const { return TetrominoePool; }
	const std::shared_ptr<Tetrominoe>& GetActiveTetrominoe() const { return ActiveTetrominoe; }
	// --- Delegate
	typedef std::function<void(Tetrominoe* const)> DelCheckRowCompletion;
	DelCheckRowCompletion CheckRowCompletionEvent;

	typedef std::function<void()> DelGenerateRandomTetrominoe;
	DelGenerateRandomTetrominoe GenerateRandomTetrominoeEvent;
	// ---
};
#endif
