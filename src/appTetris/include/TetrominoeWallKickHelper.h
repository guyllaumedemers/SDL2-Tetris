#ifndef INCLUDED_TETROMINOE_WALL_KICK_HELPER
#define INCLUDED_TETROMINOE_WALL_KICK_HELPER

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

#ifndef INCLUDED_TETROMINOE_WALL_KICK_DATA
#define INCLUDED_TETROMINOE_WALL_KICK_DATA
/// <summary>
/// Collection Wall Kick Realignments
/// </summary>
struct TetrominoeWallKicks final
{
	/// <summary>
	/// Single Wall Kick Realignment
	/// </summary>
	struct WallKickRealignment final
	{
		int8_t x = 0;
		int8_t y = 0;
	};

	std::vector<WallKickRealignment> WallKickRealignmentData = std::vector<WallKickRealignment>();
};
#endif

#include "TetrominoeShapeEnum.h"

class TetrominoeWallKickHelper final
{
	std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>> WallKickRealignmentMap = std::unordered_map<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>>();
	static std::shared_ptr<TetrominoeWallKickHelper> Singleton;
	TetrominoeShapeEnum JLTSZ = TetrominoeShapeEnum::None;
	TetrominoeShapeEnum I = TetrominoeShapeEnum::None;

	TetrominoeWallKickHelper();
public:
	// --- Getter/Setter
	static TetrominoeWallKickHelper* Get();
	const TetrominoeWallKicks& TryWallKickRealignment(class Tetrominoe* TetrominoePtrArg);
	// ---
};
#endif