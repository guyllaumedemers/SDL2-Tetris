#include "../include/Tetrominoe.h"
#include "../include/Tile.h"
#include "../include/TetrominoeWallKickHelper.h"
#include "../include/TetrominoeRotationRealignmentHelper.h"
#include "../include/SDLlogHelper.h"

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <algorithm>
#endif

#ifndef RANGE_H
#define RANGE_H
#include <ranges>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

Tetrominoe::Tetrominoe(TetrominoeShapeEnum TetrominoeEnum, uint8_t Rows, uint8_t Cols)
{
	static const uint8_t& Zero = 0;
	static const uint8_t& One = 1;
	static const uint8_t& Two = 2;
	static const uint8_t& Three = 3;

	switch (TetrominoeEnum)
	{
	case TetrominoeShapeEnum::None:
		TetrominoeEntryIndices = { Zero, Zero, Zero, Zero };
		break;
	case TetrominoeShapeEnum::TShape:
		TetrominoeEntryIndices = { One, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case TetrominoeShapeEnum::LShape:
		TetrominoeEntryIndices = { Two, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case TetrominoeShapeEnum::ZShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case TetrominoeShapeEnum::OShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	case TetrominoeShapeEnum::IShape:
		TetrominoeEntryIndices = { Zero, One, Two, Three };
		break;
	case TetrominoeShapeEnum::JShape:
		TetrominoeEntryIndices = { Zero, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case TetrominoeShapeEnum::SShape:
		TetrominoeEntryIndices = { One, Two, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	default:
		break;
	}

	TetrominoeShape = TetrominoeEnum;
	RotationIndex = 0;
	Align();
}

bool Tetrominoe::IsMoveInBound(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static const bool& IsMoveInBound = true;
	static const uint8_t& Zero = 0;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const bool& CanMoveRow = (((TetrominoeEntryIndex / Cols) + std::abs(DirY)) >= Zero && ((TetrominoeEntryIndex / Cols) + std::abs(DirY)) < Rows);
		const bool& CanMoveCol = (((TetrominoeEntryIndex % Cols) + DirX) >= Zero && ((TetrominoeEntryIndex % Cols) + DirX) < Cols);

		if (!CanMoveRow || !CanMoveCol)
		{
			return !IsMoveInBound;
		}
	}

	return IsMoveInBound;
}

bool Tetrominoe::IsMoveOverlappingExistingTile(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static const bool& IsMoveOverlappingExistingTile = true;

	if (Tiles.empty())
	{
		return !IsMoveOverlappingExistingTile;
	}

	try
	{
		const int8_t& JumpValue = (DirX + (std::abs(DirY) * Cols));

		for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			const bool& IsJumpIndexOverlappingItsOwnIndex = std::find(
				TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(), TetrominoeEntryIndex + JumpValue) != TetrominoeEntryIndices.end();

			if (IsJumpIndexOverlappingItsOwnIndex)
			{
				continue;
			}

			const Tile& Tile = Tiles.at(TetrominoeEntryIndex + JumpValue);
			if (Tile.Attribute != TileAttributeEnum::Empty)
			{
				return IsMoveOverlappingExistingTile;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}

	return !IsMoveOverlappingExistingTile;
}

void Tetrominoe::Update(std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	if (Tiles.empty())
	{
		return;
	}

	static const TileAttributeEnum& FilledEnum = TileAttributeEnum::Filled;
	static const TileAttributeEnum& EmptyEnum = TileAttributeEnum::Empty;
	static const std::string& UndefinedString = std::string("Undefined");

	try
	{
		const std::string& Wildcard = GetTetrominoeWildcard();
		const int8_t& JumpValue = (DirX + (std::abs(DirY) * Cols));

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& PreviousTile = Tiles.at(TetrominoeEntryIndex);

			PreviousTile.SetAttribute(EmptyEnum);
			PreviousTile.SetWildcard(UndefinedString);

			TetrominoeEntryIndex += JumpValue;
		}

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& NextTile = Tiles.at(TetrominoeEntryIndex);

			NextTile.SetAttribute(FilledEnum);
			NextTile.SetWildcard(Wildcard);
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}
}

void Tetrominoe::Flip(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	if (IsLocked())
	{
		return;
	}

	FlipDataHandle FlipDataHandle(TetrominoeEntryIndices.size());
	FlipDataHandle.MatrixPivot = TryFindPivot(Rows, Cols);
	FlipDataHandle.Matrix = TryCreateFlipMatrix(FlipDataHandle, Rows, Cols);
	TryRealignAndUpdate(Tiles, FlipDataHandle, Rows, Cols);
}

void Tetrominoe::Align()
{
	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		TetrominoeEntryIndex += SpawnPosition;
	}
}

void Tetrominoe::Realign(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	static const int8_t& Zero = 0;
	static const int8_t& OneDown = -1;

	while (IsMoveInBound(Zero, OneDown, Rows, Cols) && !IsMoveOverlappingExistingTile(Tiles, Zero, OneDown, Rows, Cols))
	{
		Update(Tiles, Zero, OneDown, Rows, Cols);
	}
}

uint16_t Tetrominoe::TryFindPivot(uint8_t Rows, uint8_t Cols)
{
	uint8_t MinRow = UINT8_MAX;
	uint8_t MinCol = UINT8_MAX;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const uint8_t& Col = (TetrominoeEntryIndex % Cols);
		const uint8_t& Row = (TetrominoeEntryIndex / Cols);

		if (Col < MinCol) MinCol = Col;
		if (Row < MinRow) MinRow = Row;
	}

	return (MinRow * Cols) + MinCol;
}

std::vector<int16_t> Tetrominoe::TryCreateFlipMatrix(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static const int8_t& Zero = 0;
	static const int8_t& One = 1;
	static const int8_t& Two = 2;
	static const int8_t& MinusOne = -1;

	const uint16_t& Pivot = FlipDataHandle.MatrixPivot;
	const uint8_t& NMatrix = FlipDataHandle.NMatrix;

	std::vector<int16_t> Matrix(NMatrix * NMatrix, MinusOne);

	try
	{
		for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
		{
			const uint8_t& Col = N % NMatrix;
			const uint8_t& Row = N / NMatrix;

			if (Col < Row)
			{
				continue;
			}

			// flip matrix col, row

			const uint16_t& ColIndex = (Pivot + Col + (Row * Cols));
			const uint16_t& RowIndex = (Pivot + Row + (Col * Cols));

			const bool& IsColIndexInArray = std::find(
				TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(), ColIndex) != TetrominoeEntryIndices.end();

			const bool& IsRowIndexInArray = std::find(
				TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(), RowIndex) != TetrominoeEntryIndices.end();

			// Col Permutation

			Matrix.at(Col + (Row * NMatrix)) = IsRowIndexInArray
				? RowIndex
				: MinusOne;

			// Row Permutation

			Matrix.at(Row + (Col * NMatrix)) = IsColIndexInArray
				? ColIndex
				: MinusOne;
		}

		// flip matrix cols

		for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
		{
			if ((N % NMatrix) > ((NMatrix - One) / Two))
			{
				continue;
			}

			const uint8_t& Col = (N % NMatrix);
			const uint8_t& Row = (N / NMatrix);
			const uint8_t& ColPrime = (NMatrix - One - (N % NMatrix));

			// Front
			int16_t& Front = Matrix.at((Row * NMatrix) + Col);

			// Back
			int16_t& Back = Matrix.at((Row * NMatrix) + ColPrime);

			// Swap
			const int16_t Temp = Front;
			Front = Back;
			Back = Temp;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}

	return Matrix;
}

void Tetrominoe::TryRealignAndUpdate(std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	InvalidateTetrominoeIndicies(Tiles);
	UpdateTetrominoeRotationIndex();
	GenerateTetrominoeRealignmentData(Tiles, FlipDataHandle, Rows, Cols);
	UpdateTetrominoeEntryIndicies(FlipDataHandle, Rows, Cols);
	RevalidateTetrominoeIndicies(Tiles);
}

void Tetrominoe::InvalidateTetrominoeIndicies(std::vector<Tile>& Tiles)
{
	static const TileAttributeEnum& EmptyEnum = TileAttributeEnum::Empty;
	static const std::string& UndefinedString = std::string("Undefined");

	try
	{
		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& Tile = Tiles.at(TetrominoeEntryIndex);
			Tile.SetAttribute(EmptyEnum);
			Tile.SetWildcard(UndefinedString);
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}
}

void Tetrominoe::RevalidateTetrominoeIndicies(std::vector<Tile>& Tiles)
{
	static const TileAttributeEnum& FilledEnum = TileAttributeEnum::Filled;
	const std::string& DefinedString = GetTetrominoeWildcard();

	try
	{
		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& Tile = Tiles.at(TetrominoeEntryIndex);
			Tile.SetAttribute(FilledEnum);
			Tile.SetWildcard(DefinedString);
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}
}

void Tetrominoe::UpdateTetrominoeRotationIndex()
{
	static const uint8_t& One = 1;
	static const uint8_t& Four = 4;
	SetTetrominoeRotationIndex((GetTetrominoeRotationIndex() + One) % Four);
}

void Tetrominoe::UpdateTetrominoeEntryIndicies(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static const int8_t& Zero = 0;
	static const int8_t& MinusOne = -1;

	const std::vector<int16_t>& Matrix = FlipDataHandle.Matrix;
	const uint16_t& Pivot = FlipDataHandle.MatrixPivot;
	const uint8_t& NMatrix = FlipDataHandle.NMatrix;
	const int8_t& FloorkickRealignmentValue = FlipDataHandle.FloorkickRealignmentValue;
	const int8_t& WallkickRealignmentValue = FlipDataHandle.WallkickRealignmentValue;
	const int8_t& RotationRealignmentValue = FlipDataHandle.RotationRealignmentValue;

	try
	{
		int8_t Begin = Zero;
		int8_t N = MinusOne;

		for (const auto& MatrixEntry : Matrix)
		{
			++N;
			if (MatrixEntry == MinusOne)
			{
				continue;
			}

			if (Begin < NMatrix)
			{
				const uint8_t& Col = (N % NMatrix);
				const uint8_t& Row = (N / NMatrix);

				// calculate realignment output

				const uint16_t& RealignmentOutput = (Pivot + Col + (Row * Cols)
					+ RotationRealignmentValue
					+ FloorkickRealignmentValue
					+ WallkickRealignmentValue);

				// realign

				TetrominoeEntryIndices.at(Begin++) = RealignmentOutput;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}
}

void Tetrominoe::GenerateTetrominoeRealignmentData(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	// order matter here
	FlipDataHandle.RotationRealignmentValue = TryGetRotationalAlignmentValueAtIndex(Rows, Cols);
	FlipDataHandle.FloorkickRealignmentValue = TryGetFloorKickAlignmentValueAtRotation(Tiles, FlipDataHandle, Rows, Cols);
	FlipDataHandle.WallkickRealignmentValue = TryGetWallkickAlignmentValueAtIndex(Tiles, FlipDataHandle, Rows, Cols);
}

int8_t Tetrominoe::TryGetFloorKickAlignmentValueAtRotation(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static const int8_t& Zero = 0;
	static const int8_t& One = 1;
	static const int8_t& MinusOne = -1;
	static const int8_t& MinusTwo = -2;

	const std::vector<int16_t>& Matrix = FlipDataHandle.Matrix;
	const uint16_t& Pivot = FlipDataHandle.MatrixPivot;
	const uint8_t& NMatrix = FlipDataHandle.NMatrix;
	const int8_t& RotationRealignmentValue = FlipDataHandle.RotationRealignmentValue;

	try
	{
		int8_t N = MinusOne;

		for (const auto& MatrixEntry : Matrix)
		{
			++N;
			if (MatrixEntry == MinusOne)
			{
				continue;
			}

			const uint8_t& Col = (N % NMatrix);
			const uint8_t& Row = (N / NMatrix);

			// calculate new position

			const uint16_t& RealignmentOutput = (Pivot + Col + (Row * Cols) + RotationRealignmentValue);

			// check if the new position is lower than the grid threshld or overlap a filled tile

			const Tile& Tile = Tiles.at(RealignmentOutput);

			const bool& IsRealignmentUnderThresholdOrOverlap =
				(RealignmentOutput >= ((Rows - One) * Cols)) ||
				(Tile.Attribute == TileAttributeEnum::Filled);

			if (IsRealignmentUnderThresholdOrOverlap)
			{
				const TetrominoeShapeEnum& TetrominoeShape = GetTetrominoeShape();
				const bool& CanPerformFloorKick = static_cast<bool>(TetrominoeShape & ~TetrominoeShapeEnum::OShape);

				if (CanPerformFloorKick)
				{
					return static_cast<bool>(TetrominoeShape & ~TetrominoeShapeEnum::IShape)
						? (MinusOne * Cols)
						: (MinusTwo * Cols);
				}
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}

	return NULL;
}

int8_t Tetrominoe::TryGetWallkickAlignmentValueAtIndex(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static const int8_t& Zero = 0;
	static const int8_t& MinusOne = -1;

	const std::vector<int16_t>& Matrix = FlipDataHandle.Matrix;
	const uint16_t& Pivot = FlipDataHandle.MatrixPivot;
	const uint8_t& NMatrix = FlipDataHandle.NMatrix;
	const int8_t& FloorkickRealignmentValue = FlipDataHandle.FloorkickRealignmentValue;
	const int8_t& RotationRealignmentValue = FlipDataHandle.RotationRealignmentValue;

	try
	{
		const WallKickAlignmentContainer& WallkickAlignmentContainer = TetrominoeWallKickHelper::TryGetWallKickAlignmentContainer(this);
		const uint8_t& TetrominoeRotationIndex = GetTetrominoeRotationIndex();

		// --- typedef
		using WallKickAlignment = WallKickAlignmentContainer::WallKickAlignment;
		// ---

		const std::vector<WallKickAlignment>& WallkickAlignmentAtRotationIndex = WallKickAlignmentContainer::TryGetWallkickAlignmentAtRotationIndex(
			WallkickAlignmentContainer,
			TetrominoeRotationIndex
		);

		bool IsWallkickOverlapping = true;

		// search the wallkick translation required, if any (index 0 has no wallkicks)
		uint8_t WallkickIndex = Zero;

		while (
			WallkickIndex >= Zero
			&& WallkickIndex < WallkickAlignmentAtRotationIndex.size()
			&& IsWallkickOverlapping)
		{
			const WallKickAlignment& WallkickAlignment = WallkickAlignmentAtRotationIndex.at(WallkickIndex);

			int8_t N = MinusOne;

			for (const auto& MatrixEntry : Matrix)
			{
				++N;
				if (MatrixEntry == MinusOne)
				{
					continue;
				}

				const uint8_t& Col = (N % NMatrix);
				const uint8_t& Row = (N / NMatrix);

				// calculate new position

				uint16_t RealignmentOutput = (Pivot + Col + (Row * Cols)
					+ RotationRealignmentValue
					+ FloorkickRealignmentValue);


				RealignmentOutput += ((WallkickAlignment.y * Cols) + WallkickAlignment.x);

				// check if the new position tile without wallkicks create overlaps

				const Tile& Tile = Tiles.at(RealignmentOutput);

				// check overlaps

				const bool& IsTileOverlapping = (Tile.Attribute != TileAttributeEnum::Empty);

				IsWallkickOverlapping = IsTileOverlapping;

				if (IsTileOverlapping)
				{
					++WallkickIndex;
					break;
				}
			}
		}

		if (!IsWallkickOverlapping)
		{
			const WallKickAlignment& WallkickAlignment = WallKickAlignmentContainer::TryGetWallKickAlignmentAtIndex(
				WallkickAlignmentContainer,
				TetrominoeRotationIndex,
				WallkickIndex);

 			return WallkickAlignment.IsValid()
				? ((WallkickAlignment.y * Cols) + WallkickAlignment.x)
				: NULL;
		}
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}

	return NULL;
}

int8_t Tetrominoe::TryGetRotationalAlignmentValueAtIndex(uint8_t Rows, uint8_t Cols)
{
	// --- typedef
	using RotationRealignment = RotationalAlignmentContainer::RotationalAlignment;
	// ---

	const RotationRealignment& RotationalAlignment = RotationalAlignmentContainer::TryGetRotationAlignmentAtIndex(
		TetrominoeRotationRealignmentHelper::TryGetRotationAlignmentContainer(this),
		GetTetrominoeRotationIndex()
	);

	if (!RotationalAlignment.IsValid())
	{
		return NULL;
	}

	return ((RotationalAlignment.y * Cols) + RotationalAlignment.x);
}

std::string Tetrominoe::GetTetrominoeWildcard() const
{
	static const std::unordered_map<TetrominoeShapeEnum, std::string>& TetrominoeShapePair =
	{
		std::make_pair(TetrominoeShapeEnum::None, std::string("Undefined")),
		std::make_pair(TetrominoeShapeEnum::TShape, std::string("Purple")),
		std::make_pair(TetrominoeShapeEnum::LShape, std::string("Orange")),
		std::make_pair(TetrominoeShapeEnum::ZShape, std::string("Red")),
		std::make_pair(TetrominoeShapeEnum::OShape, std::string("Yellow")),
		std::make_pair(TetrominoeShapeEnum::IShape, std::string("Cyan")),
		std::make_pair(TetrominoeShapeEnum::JShape, std::string("Blue")),
		std::make_pair(TetrominoeShapeEnum::SShape, std::string("Green"))
	};

	const auto& TetrominoeShapePairFound = TetrominoeShapePair.find(TetrominoeShape);

	return (TetrominoeShapePairFound != TetrominoeShapePair.end())
		? TetrominoeShapePairFound->second
		: std::string("Undefined");
}