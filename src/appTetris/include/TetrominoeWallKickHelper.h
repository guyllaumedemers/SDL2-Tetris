#ifndef WALLKICKHELPER_H
#define WALLKICKHELPER_H

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

#include "SDLlogHelper.h"
#include "TraitHelper.h"

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
	UnorderedMap<uint8_t, std::vector<WallKickAlignment>> WallKickRealignmentData = UnorderedMap<uint8_t, std::vector<WallKickAlignment>>();

	WallKickAlignmentContainer(const UnorderedMap<uint8_t, std::vector<WallKickAlignment>>& WallkickMap)
	{
		WallKickRealignmentData = WallkickMap;
	}

	WallKickAlignmentContainer() {}

	bool operator==(const WallKickAlignmentContainer& rhs) const
	{
		return
			(WallKickRealignmentData == rhs.WallKickRealignmentData);
	}

#pragma warning (push)
#pragma warning (disable : 4172)

	static inline const std::vector<WallKickAlignment>& TryGetWallkickAlignmentAtRotationIndex(const WallKickAlignmentContainer& Container, uint8_t RotationIndex)
	{
		static const std::vector<WallKickAlignment>& EmptyAlignmentContainer = std::vector<WallKickAlignment>();

		if (Container.WallKickRealignmentData.empty())
		{
			return EmptyAlignmentContainer;
		}

		try
		{
			return Container.WallKickRealignmentData.at(RotationIndex);
		}
		catch (const std::out_of_range& e)
		{
			SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "WallKickAlignment", e);
		}

		return EmptyAlignmentContainer;
	}

	static inline const WallKickAlignment& TryGetWallKickAlignmentAtIndex(const WallKickAlignmentContainer& Container, uint8_t RotationIndex, uint8_t WallkickIndex)
	{
		static const WallKickAlignment& EmptyAlignment = WallKickAlignment();

		if (Container.WallKickRealignmentData.empty())
		{
			return EmptyAlignment;
		}

		try
		{
			return TryGetWallkickAlignmentAtRotationIndex(Container, RotationIndex).at(WallkickIndex);
		}
		catch (const std::out_of_range& e)
		{
			SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "WallKickAlignment", e);
		}

		return EmptyAlignment;
	}

#pragma warning (pop)
};

#include "TetrominoeShapeEnum.h"

class TetrominoeWallKickHelper final
{
	UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer> WallKickRealignmentMap = UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer>();
	static std::unique_ptr<TetrominoeWallKickHelper> Singleton;
public:
	TetrominoeWallKickHelper(const TetrominoeWallKickHelper&) = default;
	TetrominoeWallKickHelper(TetrominoeWallKickHelper&&) = default;
	TetrominoeWallKickHelper();
	~TetrominoeWallKickHelper() = default;
	TetrominoeWallKickHelper& operator==(const TetrominoeWallKickHelper&) = delete;
	TetrominoeWallKickHelper& operator==(TetrominoeWallKickHelper&&) = delete;
	static const WallKickAlignmentContainer& TryGetWallKickAlignmentContainer(class Tetrominoe* TetrominoePtrArg);
	// Getter/Setter
	const UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer>& GetWallKickRealignmentMap() const { return WallKickRealignmentMap; }
private:
	static TetrominoeWallKickHelper* Get();
};
#endif