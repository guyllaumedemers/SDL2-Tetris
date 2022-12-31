#include "../include/TetrominoeRotationRealignmentHelper.h"
#include "../include/TetrominoeShapeEnum.h"
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

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::TShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::LShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::ZShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::OShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::IShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::JShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::SShape,
				RotationalAlignmentContainer{
					RotationRealignment{0,0},
					RotationRealignment{1,0},
					RotationRealignment{3,0},
					RotationRealignment{4,0}
				}
		});
}

#pragma warning (push)
#pragma warning (disable : 4172)

const RotationalAlignmentContainer& TetrominoeRotationRealignmentHelper::TryRotationAlignmentContainer(Tetrominoe* TetrominoePtrArg) const
{
	static constexpr uint8_t&& Zero = 0;

	if (RotationRealignmentMap.size() > Zero)
	{
		const RotationalAlignmentContainer EmptyContainer;
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			const RotationalAlignmentContainer EmptyContainer;
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

	const RotationalAlignmentContainer EmptyContainer;
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
