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

	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& One = 1;
	static constexpr int8_t&& Two = 2;
	static constexpr int8_t&& MinusOne = -1;
	static constexpr int8_t&& MinusTwo = -2;
	static constexpr int8_t&& MinusThree = -3;

	static const WallKickAlignmentContainer&& JLSTZ_Wallkick = WallKickAlignmentContainer
	{
		WallKickAlignment{Zero,Zero},
		WallKickAlignment{MinusOne,Zero},
		WallKickAlignment{MinusOne,One},
		WallKickAlignment{Zero,MinusTwo},
		WallKickAlignment{MinusOne,MinusTwo},
		WallKickAlignment{One,Zero},
		WallKickAlignment{One,MinusOne},
		WallKickAlignment{Zero,Two},
		WallKickAlignment{One,Two},
		WallKickAlignment{One,One},
		WallKickAlignment{One,MinusTwo},
		WallKickAlignment{MinusOne,MinusOne},
		WallKickAlignment{MinusOne,Two},
	};

	static const WallKickAlignmentContainer&& I_Wallkick = WallKickAlignmentContainer
	{
		WallKickAlignment{Zero,Zero},
		WallKickAlignment{MinusTwo,Zero},
		WallKickAlignment{One,Zero},
		WallKickAlignment{MinusTwo,MinusOne},
		WallKickAlignment{One,Two},
		WallKickAlignment{Two,Zero},
		WallKickAlignment{MinusOne,Zero},
		WallKickAlignment{Two,One},
		WallKickAlignment{MinusOne,MinusTwo},
		WallKickAlignment{MinusOne,Two},
		WallKickAlignment{Two,MinusOne},
		WallKickAlignment{One,MinusTwo},
		WallKickAlignment{MinusTwo,One},
	};

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::TShape,
				JLSTZ_Wallkick
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::LShape,
				JLSTZ_Wallkick
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::ZShape,
				JLSTZ_Wallkick
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::OShape,
				WallKickAlignmentContainer{}
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::IShape,
				I_Wallkick
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::JShape,
				JLSTZ_Wallkick
		});

	WallKickRealignmentMap.insert(
		{
				TetrominoeShapeEnum::SShape,
				JLSTZ_Wallkick
		});
};

#pragma warning (push)
#pragma warning (disable : 4172)

const WallKickAlignmentContainer& TetrominoeWallKickHelper::TryWallKickAlignmentContainer(Tetrominoe* TetrominoePtrArg) const
{
	static const WallKickAlignmentContainer EmptyContainer = WallKickAlignmentContainer();

	if (WallKickRealignmentMap.empty())
	{
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
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
