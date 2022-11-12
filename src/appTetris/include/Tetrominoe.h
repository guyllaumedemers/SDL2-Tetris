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

#ifndef INCLUDED_TETROMINOE_ENUM
#define INCLUDED_TETROMINOE_ENUM
enum ShapeEnum
{
	None = 0,
	TShape = 1,
	LShape = 2,
	ZShape = 4,
	OShape = 8,
	IShape = 16
};
#endif

class Tetrominoe final
{
	static constexpr size_t MaxEntriesPerShape = 4;
	ShapeEnum Pattern = ShapeEnum::None;

	std::array<uint16_t, MaxEntriesPerShape> TetrominoeEntryIndices = std::array<uint16_t, MaxEntriesPerShape>();
	static constexpr uint16_t SpawnPosition = 25;

	bool bIsLocked = false;
public:
	Tetrominoe(const Tetrominoe&) = delete;
	Tetrominoe(Tetrominoe&&) = delete;
	Tetrominoe() = delete;
	Tetrominoe(ShapeEnum TetrominoeEnum = ShapeEnum::None);
	~Tetrominoe() = default;
	Tetrominoe& operator=(const Tetrominoe&) = delete;
	Tetrominoe& operator=(Tetrominoe&&) = delete;
	const bool& IsLocked() const { return bIsLocked; }
	void Update(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols);
	void Flip();
};
#endif
