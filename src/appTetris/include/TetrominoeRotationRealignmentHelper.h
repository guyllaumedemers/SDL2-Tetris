#ifndef INCLUDED_TETROMINOE_ROTATION_HELPER
#define INCLUDED_TETROMINOE_ROTATION_HELPER

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_TETROMINOE_ROTATION_DATA
#define INCLUDED_TETROMINOE_ROTATION_DATA
/// <summary>
/// Collection Rotation Realignments
/// </summary>
struct TetrominoeRotationRealignments final
{
	/// <summary>
	/// Single Rotation Realignment
	/// </summary>
	struct RotationRealignment final
	{
		int8_t x = 0;
		int8_t y = 0;
	};

	std::vector<RotationRealignment> RotationRealignmentData = std::vector<RotationRealignment>();
};
#endif

#include "TetrominoeShapeEnum.h"

class TetrominoeRotationRealignmentHelper final
{
	std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>, std::hash<TetrominoeShapeEnum>> RotationRealignmentMap = std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeRotationRealignments>, std::hash<TetrominoeShapeEnum>>();
	static std::unique_ptr<TetrominoeRotationRealignmentHelper> Singleton;
	TetrominoeShapeEnum JLTSZ = TetrominoeShapeEnum::None;
	TetrominoeShapeEnum I = TetrominoeShapeEnum::None;
	TetrominoeShapeEnum O = TetrominoeShapeEnum::None;

public:
	TetrominoeRotationRealignmentHelper();
	// --- Getter/Setter
	static TetrominoeRotationRealignmentHelper* Get();
	const TetrominoeRotationRealignments& TryRotationRealignment(class Tetrominoe* TetrominoePtrArg);
	// ---
};
#endif