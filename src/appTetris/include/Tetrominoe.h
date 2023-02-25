#ifndef TETROMINOE_H
#define TETROMINOE_H

#ifndef UMAP_H
#define UMAP_H
#include <unordered_map>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

#include "Tile.h"
#include "TetrominoeShapeEnum.h"

class Tetrominoe final
{
	struct FlipDataHandle final
	{
		int8_t FloorkickRealignmentValue = NULL;
		int8_t WallkickRealignmentValue = NULL;
		int8_t RotationRealignmentValue = NULL;
		uint16_t MatrixPivot = NULL;
		const uint8_t NMatrix = NULL;

		std::vector<int16_t> Matrix = std::vector<int16_t>();

		FlipDataHandle(uint8_t InNMatrix) : NMatrix(InNMatrix) {}
	};

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
	void Flip(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
private:
	void Align();
	void Realign(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols);
	// Flip
	uint16_t TryFindPivot(uint8_t Rows, uint8_t Cols);
	std::vector<int16_t> TryCreateFlipMatrix(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	void TryRealignAndUpdate(std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	// Utils
	void InvalidateTetrominoeIndicies(std::vector<Tile>& Tiles);
	void RevalidateTetrominoeIndicies(std::vector<Tile>& Tiles);
	void UpdateTetrominoeRotationIndex();
	void UpdateTetrominoeEntryIndicies(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	void GenerateTetrominoeRealignmentData(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	int8_t TryGetFloorKickAlignmentValueAtRotation(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	int8_t TryGetWallkickAlignmentValueAtIndex(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols);
	int8_t TryGetRotationalAlignmentValueAtIndex(uint8_t Rows, uint8_t Cols);
public:
	// Getters/Setters
	const std::vector<uint16_t>& GetTetrominoeIndices() const { return TetrominoeEntryIndices; }
	const TetrominoeShapeEnum& GetTetrominoeShape() const { return TetrominoeShape; }
	std::string GetTetrominoeWildcard() const;
	const bool& IsLocked() const { return bIsLocked; }
	void SetIsLocked() { bIsLocked = true; }
	const uint8_t& GetTetrominoeRotationIndex() const { return RotationIndex; }
	void SetTetrominoeRotationIndex(uint8_t Index) { RotationIndex = Index; }
};
#endif
