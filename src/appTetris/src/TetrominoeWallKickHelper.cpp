#include "../include/TetrominoeWallKickHelper.h"
#include "../include/Tetrominoe.h"

// --- Static Fields
std::unique_ptr<TetrominoeWallKickHelper> TetrominoeWallKickHelper::Singleton = nullptr;
// ---

TetrominoeWallKickHelper::TetrominoeWallKickHelper()
{
	// --- typedef
	using WallKickAlignment = WallKickAlignmentContainer::WallKickAlignment;
	// ---

	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& One = 1;
	static constexpr int8_t&& Two = 2;
	static constexpr int8_t&& Three = 3;
	static constexpr int8_t&& MinusOne = -1;
	static constexpr int8_t&& MinusTwo = -2;

	static const WallKickAlignmentContainer&& JLSTZ_Wallkick = WallKickAlignmentContainer
	{
		{
			// Clockwise Only
			{ Zero,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{MinusOne,Zero},
						WallKickAlignment{MinusOne,One},
						WallKickAlignment{Zero,MinusTwo},
						WallKickAlignment{MinusOne,MinusTwo}}
			},
			{ One,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{One,Zero},
						WallKickAlignment{One,MinusOne},
						WallKickAlignment{Zero,Two},
						WallKickAlignment{One,Two}}
			},
			{ Two,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{One,Zero},
						WallKickAlignment{One,One},
						WallKickAlignment{Zero,MinusTwo},
						WallKickAlignment{One,MinusTwo}}
			},
			{ Three,std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{MinusOne,Zero},
						WallKickAlignment{MinusOne,MinusOne},
						WallKickAlignment{Zero,Two},
						WallKickAlignment{MinusOne,MinusTwo}}
			}
		}
	};

	static const WallKickAlignmentContainer&& I_Wallkick = WallKickAlignmentContainer
	{
		{
			// Clockwise Only
			{ Zero,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{MinusTwo,Zero},
						WallKickAlignment{One,Zero},
						WallKickAlignment{MinusTwo,MinusOne},
						WallKickAlignment{One,Two}}
			},
			{ One,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{MinusOne,Zero},
						WallKickAlignment{Two,Zero},
						WallKickAlignment{MinusOne,Two},
						WallKickAlignment{Two,MinusOne}}
			},
			{ Two,	std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{Two,Zero},
						WallKickAlignment{MinusOne,Zero},
						WallKickAlignment{Two,One},
						WallKickAlignment{MinusOne,MinusTwo}}
			},
			{ Three,std::vector<WallKickAlignment>{
						WallKickAlignment{Zero,Zero},
						WallKickAlignment{One,Zero},
						WallKickAlignment{MinusTwo,Zero},
						WallKickAlignment{One,MinusTwo},
						WallKickAlignment{MinusTwo,One}}
			}
		}
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

const WallKickAlignmentContainer& TetrominoeWallKickHelper::TryGetWallKickAlignmentContainer(Tetrominoe* TetrominoePtrArg)
{
	const UnorderedMap<TetrominoeShapeEnum, WallKickAlignmentContainer>& WallKickRealignmentMapInstance = Get()->GetWallKickRealignmentMap();
	static const WallKickAlignmentContainer EmptyContainer = WallKickAlignmentContainer();

	if (WallKickRealignmentMapInstance.empty())
	{
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return EmptyContainer;
		}

		const auto Iterator = WallKickRealignmentMapInstance.find(TetrominoePtrArg->GetTetrominoeShape());
		if (Iterator != WallKickRealignmentMapInstance.end())
		{
			return Iterator->second;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "WallKickAlignmentContainer", e);
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
		Singleton = std::make_unique<TetrominoeWallKickHelper>();
	}
	return Singleton.get();
}
