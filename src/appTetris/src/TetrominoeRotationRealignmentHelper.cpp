#include "../include/TetrominoeRotationRealignmentHelper.h"
#include "../include/Tetrominoe.h"

// --- Static Fields
std::unique_ptr<TetrominoeRotationRealignmentHelper> TetrominoeRotationRealignmentHelper::Singleton = nullptr;
// ---

TetrominoeRotationRealignmentHelper::TetrominoeRotationRealignmentHelper()
{
	// --- typedef
	using RotationRealignment = RotationalAlignmentContainer::RotationalAlignment;
	// ---

	static const int8_t& Zero = 0;
	static const int8_t& MinusOne = -1;
	static const int8_t& MinusTwo = -2;
	static const int8_t& MinusThree = -3;

	/// <summary>
	/// Shared Rotational Offset => limit code duplication using static fields but would have
	/// prefered using bitmask for HashKey if the operator() overload would have passed current hash
	/// lookup as reference, we could have run a bitwise AND operator on it. Would have reduce the number of key
	/// entries from 7 to 3.
	/// </summary>
	static const RotationalAlignmentContainer&& JLSTZ_Rot = RotationalAlignmentContainer
	{
		RotationRealignment{MinusOne,Zero},
		RotationRealignment{MinusTwo,Zero}
	};

	static const RotationalAlignmentContainer&& O_Rot = RotationalAlignmentContainer
	{
		RotationRealignment{MinusTwo,Zero}
	};

	static const RotationalAlignmentContainer&& I_Rot = RotationalAlignmentContainer
	{
		RotationRealignment{Zero,Zero},
		RotationRealignment{MinusThree,MinusOne}
	};

	/// <summary>
	/// Clockwise ONLY
	/// </summary>
	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::JShape,
				JLSTZ_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::LShape,
				JLSTZ_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::SShape,
				JLSTZ_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::TShape,
				JLSTZ_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::ZShape,
				JLSTZ_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::OShape,
				O_Rot
		});

	RotationRealignmentMap.insert(
		{
				TetrominoeShapeEnum::IShape,
				I_Rot
		});
}

#pragma warning (push)
#pragma warning (disable : 4172)

const RotationalAlignmentContainer& TetrominoeRotationRealignmentHelper::TryGetRotationAlignmentContainer(Tetrominoe* TetrominoePtrArg)
{
	const UnorderedMap<const TetrominoeShapeEnum, const RotationalAlignmentContainer&>& RotationRealignmentMapInstance = Get()->GetRotationRealignmentMap();
	static const RotationalAlignmentContainer& EmptyContainer = RotationalAlignmentContainer();

	if (RotationRealignmentMapInstance.empty())
	{
		return EmptyContainer;
	}

	try
	{
		if (!TetrominoePtrArg)
		{
			return EmptyContainer;
		}

		const auto Iterator = RotationRealignmentMapInstance.find(TetrominoePtrArg->GetTetrominoeShape());
		if (Iterator != RotationRealignmentMapInstance.end())
		{
			return Iterator->second;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "TetrominoeRotationRealignmentHelper", e);
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
		Singleton = std::make_unique<TetrominoeRotationRealignmentHelper>();
	}
	return Singleton.get();
}
