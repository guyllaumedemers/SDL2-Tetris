#ifndef INCLUDED_TETROMINOE
#define INCLUDED_TETROMINOE

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#include "Shape.h"

class Tetrominoe final
{
	static constexpr uint8_t SpawnPosition = 25;
	Shape TetrominoeShape;
	bool bIsLocked = false;

public:
	Tetrominoe(ShapeEnum TetrominoeEnum = ShapeEnum::None);
	~Tetrominoe() = default;

	Shape& GetTetrominoe() { return TetrominoeShape; }
	const bool& IsLocked() const { return bIsLocked; }
};
#endif
