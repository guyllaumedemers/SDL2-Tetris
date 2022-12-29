#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#include "Tile.h"
#include "TetrominoeShapeEnum.h"

class Tetrominoe final
{
	static constexpr uint16_t&& SpawnPosition = 15;
	static inline uint8_t&& RotationIndex = 0;

	TetrominoeShapeEnum TetrominoeShape = TetrominoeShapeEnum::None;
	std::vector<uint16_t> TetrominoeEntryIndices = std::vector<uint16_t>();
	bool bIsLocked = false;
public:
	Tetrominoe(const Tetrominoe&) = delete;
	Tetrominoe(Tetrominoe&&) = delete;
	Tetrominoe() = delete;
	Tetrominoe(TetrominoeShapeEnum TetrominoeEnum, uint8_t Rows, uint8_t Cols);
	~Tetrominoe() = default;
	Tetrominoe& operator=(const Tetrominoe&) = delete;
	Tetrominoe& operator=(Tetrominoe&&) = delete;
	bool IsMoveInBound(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const;
	bool IsMoveOverlappingExistingTile(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const;
	void Update(std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);
	void FlipMatrix(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	void Realign(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	void Align();
	// --- Getter/Setter
	const std::vector<uint16_t>& GetTetrominoeIndices() const { return TetrominoeEntryIndices; }
	const TetrominoeShapeEnum& GetTetrominoeShape() const { return TetrominoeShape; }
	std::string GetTetrominoeWildcard() const;
	const bool& IsLocked() const { return bIsLocked; }
	void SetIsLocked() { bIsLocked = true; }
	const uint8_t& GetTetrominoeRotationIndex() const { return RotationIndex; }
	void SetTetrominoeRotationIndex(uint8_t Index) { RotationIndex = Index; }
	// ---
};
#endif
