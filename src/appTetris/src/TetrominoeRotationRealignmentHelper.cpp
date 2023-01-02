#include "../include/TetrominoeRotationRealignmentHelper.h"
#include "../include/Tetrominoe.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
#endif

// --- Static Fields
std::shared_ptr<TetrominoeRotationRealignmentHelper> TetrominoeRotationRealignmentHelper::Singleton = nullptr;
// ---

TetrominoeRotationRealignmentHelper::TetrominoeRotationRealignmentHelper()
{
	// --- typedef
	using RotationRealignment = RotationalAlignmentContainer::RotationalAlignment;
	// ---

	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& One = 1;
	static constexpr int8_t&& Two = 2;
	static constexpr int8_t&& MinusOne = -1;

	/// <summary>
	/// Shared Rotational Offset
	/// </summary>
	static const TetrominoeShapeEnum JLSTZ =
		(TetrominoeShapeEnum::JShape | TetrominoeShapeEnum::LShape | TetrominoeShapeEnum::SShape | TetrominoeShapeEnum::TShape | TetrominoeShapeEnum::ZShape);

	static const TetrominoeShapeEnum O =
		(TetrominoeShapeEnum::OShape);

	static const TetrominoeShapeEnum I =
		(TetrominoeShapeEnum::IShape);

	/// <summary>
	/// Clockwise ONLY
	/// </summary>
	RotationRealignmentMap.insert(
		{
				JLSTZ,
				RotationalAlignmentContainer{
					RotationRealignment{Zero,Zero},
					RotationRealignment{One,Zero},
					RotationRealignment{One,MinusOne},
					RotationRealignment{Zero,Two},
					RotationRealignment{One,Two}
				}
		});

	RotationRealignmentMap.insert(
		{
				O,
				RotationalAlignmentContainer{
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero}
				}
		});

	RotationRealignmentMap.insert(
		{
				I,
				RotationalAlignmentContainer{
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero},
					RotationRealignment{Zero,Zero}
				}
		});
}

#pragma warning (push)
#pragma warning (disable : 4172)

const RotationalAlignmentContainer& TetrominoeRotationRealignmentHelper::TryRotationAlignmentContainer(Tetrominoe* TetrominoePtrArg) const
{
	static const RotationalAlignmentContainer&& EmptyContainer = RotationalAlignmentContainer();

	if (RotationRealignmentMap.empty())
	{
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return EmptyContainer;
		}

		const auto Iterator = RotationRealignmentMap.find(TetrominoePtrArg->GetTetrominoeShape());
		if (Iterator != RotationRealignmentMap.end())
		{
			return Iterator->second;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY REALIGNMENTOUTCOME AT ROTATION INDEX FUNCTION! %s", e.what());
	}

	return EmptyContainer;
}

#pragma warning (pop)

TetrominoeRotationRealignmentHelper* TetrominoeRotationRealignmentHelper::Get()
{
	/// <summary>
	/// cannot use make_shared or make_unique with private constructor level
	/// </summary>
	/// <returns></returns>
	if (!Singleton)
	{
		Singleton.reset(new TetrominoeRotationRealignmentHelper());
	}
	return Singleton.get();
}
