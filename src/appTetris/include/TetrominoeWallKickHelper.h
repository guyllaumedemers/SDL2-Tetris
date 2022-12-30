#ifndef INCLUDED_TETROMINOE_WALL_KICK_HELPER
#define INCLUDED_TETROMINOE_WALL_KICK_HELPER

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_CSTD_LIB
#define INCLUDED_CSTD_LIB
#include <cstdlib>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#ifndef INCLUDED_TYPE_TRAIT
#define INCLUDED_TYPE_TRAIT
#include <type_traits>
#endif

#ifndef INCLUDED_COLLECTION_UNORDERED_MAP
#define INCLUDED_COLLECTION_UNORDERED_MAP
#include <unordered_map>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
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

		inline bool operator==(const WallKickRealignment& rhs)
		{
			return
				(x == rhs.x) &&
				(y == rhs.y);
		}
	};

	std::vector<WallKickRealignment> WallKickRealignmentData = std::vector<WallKickRealignment>();

	inline bool operator==(const TetrominoeWallKicks& rhs)
	{
		return
			(WallKickRealignmentData == rhs.WallKickRealignmentData);
	}
};
#endif

#include "TetrominoeShapeEnum.h"

class TetrominoeWallKickHelper final
{
	struct EnumHash
	{
		template<typename T>
		std::size_t operator()(const T& Val) const
		{
			return (std::size_t)(const_cast<T>(Val));
		}
	};

	template<typename TKey>
	using ConditionalHash = std::conditional<std::is_enum<TKey>::value, EnumHash, std::hash<TKey>>::type;

	struct EnumEquality
	{
		template<typename T>
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return (bool)((int)(const_cast<T>(lhs)) & (int)(const_cast<T>(rhs)));
		}
	};

	template<typename TKey>
	using ConditionalEquality = std::conditional<std::is_enum<TKey>::value, EnumEquality, std::equal_to<TKey>>::type;

	template<typename TKey, typename TVal>
	using UnorderedMap = std::unordered_map<TKey, TVal, ConditionalHash<TKey>, ConditionalEquality<TKey>>;

	UnorderedMap<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>> WallKickRealignmentMap = UnorderedMap<TetrominoeShapeEnum, std::vector<TetrominoeWallKicks>>();
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