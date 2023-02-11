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

#ifndef INCLUDED_THREAD_MUTEX
#define INCLUDED_THREAD_MUTEX
#include <mutex>
#endif

#include "Tetrominoe.h"

class TetrominoeManager final
{
	std::vector<std::shared_ptr<Tetrominoe>> TetrominoePool = std::vector<std::shared_ptr<Tetrominoe>>();
	std::shared_ptr<Tetrominoe> ActiveTetrominoe = nullptr;
	std::mutex ActiveTetrominoeMutex;
	int LockDelayID = INT32_MIN;

	void Add(uint8_t Rows, uint8_t Cols);
	void Remove();
	std::unique_ptr<Tetrominoe> GenerateRandomTetromioeShape(uint8_t Rows, uint8_t Cols) const;
	void ClearTetrominoesOnRow(uint8_t Rows, uint8_t Cols, size_t TetrominoeIndex);
	void RealignTetrominoes(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols) const;
	void ResetLockDelay();
public:
	TetrominoeManager(const TetrominoeManager&) = delete;
	TetrominoeManager(TetrominoeManager&&) = delete;
	TetrominoeManager() = default;
	~TetrominoeManager() = default;
	TetrominoeManager& operator=(const TetrominoeManager&) = delete;
	TetrominoeManager& operator=(TetrominoeManager&&) = delete;
	void Initialize(const std::vector<Tile>& Tiles, const uint8_t& Rows, const uint8_t& Cols, std::function<bool(uint16_t)> RowCompletionCallback);
	void Update(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);
	void Flip(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	void Clear();
	// --- Getter/Setter
	const std::vector<std::shared_ptr<Tetrominoe>>& GetTetrominoes() const { return TetrominoePool; }
	// --- Delegate
	typedef std::function<void(Tetrominoe* const)> DelCheckRowCompletion;
	DelCheckRowCompletion CheckRowCompletionEvent;

	typedef std::function<void()> DelGenerateRandomTetrominoe;
	DelGenerateRandomTetrominoe GenerateRandomTetrominoeEvent;

	typedef std::function<void()> DelLockActiveTetrominoe;
	DelLockActiveTetrominoe LockActiveTetrominoeEvent;

	typedef std::function<int(int)> DelResetTetrominoeLockDelayTimer;
	DelResetTetrominoeLockDelayTimer ResetTetrominoeLockDelayTimerEvent;
	// ---
};
#endif
