#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_COLLECTION_ARRAY
#define INCLUDED_COLLECTION_ARRAY
#include <array>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#ifndef INCLUDED_TETROMINOE_ENUM
#define INCLUDED_TETROMINOE_ENUM
enum TetrominoeShapeEnum
{
	None = 0,
	TShape = 1,
	LShape = 2,
	ZShape = 3,
	OShape = 4,
	IShape = 5,
	JShape = 6,
	SShape = 7
};
#endif

#include "Tile.h"

class Tetrominoe final
{
	static constexpr size_t MaxEntriesPerShape = 4;
	static constexpr uint16_t SpawnPosition = 25;

	TetrominoeShapeEnum TetrominoeShape = TetrominoeShapeEnum::None;
	std::array<uint16_t, MaxEntriesPerShape> TetrominoeEntryIndices = std::array<uint16_t, MaxEntriesPerShape>();
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
	void FlipClockwise(uint8_t Rows, uint8_t Cols);
	void Realign(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	// --- Getter/Setter
	const std::array<uint16_t, MaxEntriesPerShape>& GetTetrominoeIndices() const { return TetrominoeEntryIndices; }
	const TetrominoeShapeEnum& GetTetrominoeShape() const { return TetrominoeShape; }
	std::string GetTetrominoeWildcard() const;
	const bool& IsLocked() const { return bIsLocked; }
	void SetIsLocked() { bIsLocked = true; }
	// ---
};
#endif
