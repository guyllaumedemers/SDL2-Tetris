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

#ifndef INCLUDED_TETROMINOE_ROTATION_DATA
#define INCLUDED_TETROMINOE_ROTATION_DATA

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

struct RotationalAlignmentContainer final
{
	/// <summary>
	/// Single Alignment which follows a Rotation
	/// </summary>
	struct RotationalAlignment final
	{
		int8_t x = INT8_MAX;
		int8_t y = INT8_MAX;

		bool operator==(const RotationalAlignment& rhs) const
		{
			return
				(x == rhs.x) &&
				(y == rhs.y);
		}
	};

	/// <summary>
	/// Realignment possible following rotation of tetrominoe which keep pivot point unchanged
	/// </summary>
	std::vector<RotationalAlignment> RotationalRealignmentData = std::vector<RotationalAlignment>();

	RotationalAlignmentContainer(std::initializer_list<RotationalAlignment> Alignments)
	{
		for (const auto& Alignment : Alignments)
		{
			RotationalRealignmentData.push_back(Alignment);
		}
	}

	RotationalAlignmentContainer() {}

	bool operator==(const RotationalAlignmentContainer& rhs) const
	{
		return
			(RotationalRealignmentData == rhs.RotationalRealignmentData);
	}

#pragma warning (push)
#pragma warning (disable : 4172)

	inline const RotationalAlignment& TryGetRotationAlignmentAtIndex(uint8_t Index) const
	{
		static constexpr uint8_t&& Zero = 0;

		if (RotationalRealignmentData.size() > Zero)
		{
			const RotationalAlignment InvalidRealignment;
			return InvalidRealignment;
		}

		try
		{
			return RotationalRealignmentData.at(Index);
		}
		catch (const std::out_of_range& e)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY GET_ROTATIONAL_REALIGNMENT AT ROTATION INDEX FUNCTION! %s", e.what());
		}

		const RotationalAlignment InvalidRealignment;
		return InvalidRealignment;
	}

#pragma warning (pop)
};
#endif

#include "TetrominoeShapeEnum.h"
#include "TraitHelper.h"

class TetrominoeRotationRealignmentHelper final
{
	UnorderedMap<TetrominoeShapeEnum, RotationalAlignmentContainer> RotationRealignmentMap = UnorderedMap<TetrominoeShapeEnum, RotationalAlignmentContainer>();
	static std::shared_ptr<TetrominoeRotationRealignmentHelper> Singleton;

	TetrominoeRotationRealignmentHelper();
public:
	// --- Getter/Setter
	static TetrominoeRotationRealignmentHelper* Get();
	const RotationalAlignmentContainer& TryRotationAlignmentContainer(class Tetrominoe* TetrominoePtrArg) const;
	// ---
};
#endif