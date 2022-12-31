#include "../include/TetrominoeWallKickHelper.h"
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
std::shared_ptr<TetrominoeWallKickHelper> TetrominoeWallKickHelper::Singleton = nullptr;
// ---

TetrominoeWallKickHelper::TetrominoeWallKickHelper()
{
	// --- typedef
	using WallKickAlignment = WallKickAlignmentContainer::WallKickAlignment;
	// ---

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::TShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::LShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::ZShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::OShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::IShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::JShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::SShape,
				WallKickAlignmentContainer{
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0},
					WallKickAlignment{0,0}
				}
		});
};

#pragma warning (push)
#pragma warning (disable : 4172)

const WallKickAlignmentContainer& TetrominoeWallKickHelper::TryWallKickAlignmentContainer(Tetrominoe* TetrominoePtrArg) const
{
	static constexpr uint8_t&& Zero = 0;

	if (WallKickRealignmentMap.size() > Zero)
	{
		const WallKickAlignmentContainer EmptyContainer;
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			const WallKickAlignmentContainer EmptyContainer;
			return EmptyContainer;
		}

		const auto Iterator = WallKickRealignmentMap.find(TetrominoePtrArg->GetTetrominoeShape());
		if (Iterator != WallKickRealignmentMap.end())
		{
			return Iterator->second;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN TRY REALIGNMENTOUTCOME AT ROTATION INDEX FUNCTION! %s", e.what());
	}

	const WallKickAlignmentContainer EmptyContainer;
	return EmptyContainer;
}

#pragma warning (pop)

TetrominoeWallKickHelper* TetrominoeWallKickHelper::Get()
{
	/// <summary>
	/// cannot use make_shared or make_unique with private constructor level
	/// </summary>
	/// <returns></returns>
	if (!Singleton)
	{
		Singleton.reset(new TetrominoeWallKickHelper());
	}
	return Singleton.get();
}
