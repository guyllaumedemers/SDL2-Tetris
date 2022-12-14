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

#ifndef INCLUDED_TETROMINOE_WALL_KICK_DATA
#define INCLUDED_TETROMINOE_WALL_KICK_DATA

#ifndef INCLUDED_COLLECTION_VECTOR
#define INCLUDED_COLLECTION_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
#endif

struct WallKickAlignmentContainer final
{
	/// <summary>
	/// Single Wall Kick Realignment
	/// </summary>
	struct WallKickAlignment final
	{
		int8_t x = INT8_MAX;
		int8_t y = INT8_MAX;

		bool operator==(const WallKickAlignment& rhs) const
		{
			return
				(x == rhs.x) &&
				(y == rhs.y);
		}

		constexpr bool IsValid() const { return ((x >= INT8_MIN) && (x < INT8_MAX)) && ((y >= INT8_MIN) && (y < INT8_MAX)); }
	};

	/// <summary>
	/// Wall Kicks possible following rotation of tetrominoe which overlap on occupied tile
	/// </summary>
	std::vector<WallKickAlignment> WallKickRealignmentData = std::vector<WallKickAlignment>();

	WallKickAlignmentContainer(std::initializer_list<WallKickAlignment> Alignments)
	{
		for (const auto& Alignment : Alignments)
		{
			WallKickRealignmentData.push_back(Alignment);
		}
	}

	WallKickAlignmentContainer() {}

	bool operator==(const WallKickAlignmentContainer& rhs) const
	{
		return
			(WallKickRealignmentData == rhs.WallKickRealignmentData);
	}

#pragma warning (push)
#pragma warning (disable : 4172)

	inline const WallKickAlignment& TryGetWallKickAlignmentAtIndex(uint8_t Index) const
	{
		static constexpr WallKickAlignment&& EmptyAlignment = WallKickAlignment();

		if (WallKickRealignmentData.empty())
		{
			return EmptyAlignment;
		}

		try
		{
			return WallKickRealignmentData.at(Index);
		}
		catch (const std::out_of_range& e)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY GET_WALL_KICK_REALIGNMENT AT ROTATION INDEX FUNCTION! %s", e.what());
		}

		return EmptyAlignment;
	}

#pragma warning (pop)
};
#endif

#include "TetrominoeShapeEnum.h"
#include "TraitHelper.h"

class TetrominoeWallKickHelper final
{
	UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer> WallKickRealignmentMap = UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer>();
	static std::shared_ptr<TetrominoeWallKickHelper> Singleton;

	TetrominoeWallKickHelper();
public:
	// --- Getter/Setter
	static TetrominoeWallKickHelper* Get();
	const WallKickAlignmentContainer& TryWallKickAlignmentContainer(class Tetrominoe* TetrominoePtrArg) const;
	// ---
};
#endif