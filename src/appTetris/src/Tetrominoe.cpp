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
	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;
	static constexpr uint8_t&& Two = 2;
	static constexpr uint8_t&& Three = 3;

	switch (TetrominoeEnum)
	{
	case TetrominoeShapeEnum::None:
		TetrominoeEntryIndices = { Zero, Zero, Zero, Zero };
		break;
	case TetrominoeShapeEnum::TShape:
		TetrominoeEntryIndices = { static_cast<uint8_t>(One), static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
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
	static constexpr bool&& IsMoveInBound = true;
	static constexpr uint8_t&& Zero = 0;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const bool&& CanMoveRow = ((TetrominoeEntryIndex / Cols) + std::abs(DirY)) >= Zero && ((TetrominoeEntryIndex / Cols) + std::abs(DirY)) < Rows;
		const bool&& CanMoveCol = ((TetrominoeEntryIndex % Cols) + DirX) >= Zero && ((TetrominoeEntryIndex % Cols) + DirX) < Cols;

		if (!CanMoveRow || !CanMoveCol)
		{
			return !IsMoveInBound;
		}
	}

	return IsMoveInBound;
}

bool Tetrominoe::IsMoveOverlappingExistingTile(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static constexpr bool&& IsMoveOverlappingExistingTile = true;

	if (Tiles.empty())
	{
		return !IsMoveOverlappingExistingTile;
	}

	try
	{
		const int8_t&& JumpValue = static_cast<int8_t>(DirX + (std::abs(DirY) * Cols));

		for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			const bool&& IsJumpIndexOverlappingItsOwnIndex = std::find(
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

	try
	{
		const int8_t&& JumpValue = static_cast<int8_t>(DirX + (std::abs(DirY) * Cols));
		const std::string&& Wildcard = GetTetrominoeWildcard();

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& PreviousTile = Tiles.at(TetrominoeEntryIndex);

			PreviousTile.SetAttribute(TileAttributeEnum::Empty);
			PreviousTile.SetWildcard("Undefined");

			TetrominoeEntryIndex += JumpValue;
		}

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			Tile& NextTile = Tiles.at(TetrominoeEntryIndex);

			NextTile.SetAttribute(TileAttributeEnum::Filled);
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
	RealignAndUpdate(Tiles, FlipDataHandle, Rows, Cols);
}

void Tetrominoe::Realign(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& OneDown = -1;

	while (IsMoveInBound(Zero, OneDown, Rows, Cols) && !IsMoveOverlappingExistingTile(Tiles, Zero, OneDown, Rows, Cols))
	{
		Update(Tiles, Zero, OneDown, Rows, Cols);
	}
}

void Tetrominoe::Align()
{
	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		TetrominoeEntryIndex += SpawnPosition;
	}
}

uint16_t Tetrominoe::TryFindPivot(uint8_t Rows, uint8_t Cols)
{
	uint8_t&& MinRow = UINT8_MAX;
	uint8_t&& MinCol = UINT8_MAX;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const uint8_t&& Col = static_cast<uint8_t>(TetrominoeEntryIndex % Cols);
		const uint8_t&& Row = static_cast<uint8_t>(TetrominoeEntryIndex / Cols);

		if (Col < MinCol) MinCol = Col;
		if (Row < MinRow) MinRow = Row;
	}

	return static_cast<uint16_t>((MinRow * Cols) + MinCol);
}

std::vector<int16_t> Tetrominoe::TryCreateFlipMatrix(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& One = 1;
	static constexpr int8_t&& Two = 2;
	static constexpr int8_t&& MinusOne = -1;

	const uint16_t& Pivot = FlipDataHandle.MatrixPivot;
	const uint8_t& NMatrix = FlipDataHandle.NMatrix;

	std::vector<int16_t> Matrix(NMatrix * NMatrix, MinusOne);

	try
	{
		for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
		{
			const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
			const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);

			if (Col < Row)
			{
				continue;
			}

			// flip matrix col, row

			const uint16_t&& ColIndex = static_cast<uint16_t>(Pivot + Col + (Row * Cols));
			const uint16_t&& RowIndex = static_cast<uint16_t>(Pivot + Row + (Col * Cols));

			const bool&& IsColIndexInArray = std::find(
				TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(), ColIndex) != TetrominoeEntryIndices.end();

			const bool&& IsRowIndexInArray = std::find(
				TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(), RowIndex) != TetrominoeEntryIndices.end();

			// Col Permutation

			Matrix.at(Col + (Row * NMatrix)) = IsRowIndexInArray
				? static_cast<int16_t>(RowIndex)
				: static_cast<int16_t>(MinusOne);

			// Row Permutation

			Matrix.at(Row + (Col * NMatrix)) = IsColIndexInArray
				? static_cast<int16_t>(ColIndex)
				: static_cast<int16_t>(MinusOne);
		}

		// flip matrix cols

		for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
		{
			if ((N % NMatrix) > ((NMatrix - One) / Two))
			{
				continue;
			}

			const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
			const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);
			const uint8_t&& ColPrime = static_cast<uint8_t>(NMatrix - One - (N % NMatrix));

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

void Tetrominoe::RealignAndUpdate(std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	InvalidateTetrominoeIndicies(Tiles);
	UpdateTetrominoeRotationIndex();
	GenerateTetrominoeRealignmentData(Tiles, FlipDataHandle, Rows, Cols);
	UpdateTetrominoeEntryIndicies(FlipDataHandle, Rows, Cols);
	RevalidateTetrominoeIndicies(Tiles);
}

void Tetrominoe::InvalidateTetrominoeIndicies(std::vector<Tile>& Tiles)
{
	static constexpr TileAttributeEnum&& EmptyEnum = TileAttributeEnum::Empty;
	static const std::string&& UndefinedString = std::string("Undefined");

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
	static constexpr TileAttributeEnum&& FilledEnum = TileAttributeEnum::Filled;
	static std::string&& DefinedString = GetTetrominoeWildcard();

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
	static constexpr size_t&& One = 1;
	static constexpr size_t&& Four = 4;
	SetTetrominoeRotationIndex((GetTetrominoeRotationIndex() + One) % Four);
}

void Tetrominoe::UpdateTetrominoeEntryIndicies(const FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;

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
				const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
				const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);

				// calculate realignment output

				const uint16_t&& RealignmentOutput = static_cast<uint16_t>(Pivot + Col + (Row * Cols)
					+ RotationRealignmentValue
					+ FloorkickRealignmentValue
					+ WallkickRealignmentValue
					);

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
	FlipDataHandle.RotationRealignmentValue = GetRotationalAlignmentValueAtIndex(Rows, Cols);
	FlipDataHandle.FloorkickRealignmentValue = GetFloorKickAlignmentValueAtRotation(Tiles, FlipDataHandle, Rows, Cols);
	FlipDataHandle.WallkickRealignmentValue = GetWallkickAlignmentValueAtIndex(Tiles, FlipDataHandle, Rows, Cols);
}

int8_t Tetrominoe::GetFloorKickAlignmentValueAtRotation(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;
	static constexpr int8_t&& MinusTwo = -2;

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

			const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
			const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);

			// calculate new position

			const uint16_t&& RealignmentOutput = static_cast<uint16_t>(Pivot + Col + (Row * Cols) + RotationRealignmentValue);

			// check if the new position tile with wallkicks create overlaps

			const Tile& Tile = Tiles.at(RealignmentOutput);

			// check overlaps

			const bool&& IsTileOverlapping = (Tile.Attribute != TileAttributeEnum::Empty);

			if (IsTileOverlapping)
			{
				const TetrominoeShapeEnum& TetrominoeShape = GetTetrominoeShape();
				const bool&& CanPerformFloorKick = static_cast<bool>(TetrominoeShape & ~TetrominoeShapeEnum::OShape);

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

int8_t Tetrominoe::GetWallkickAlignmentValueAtIndex(const std::vector<Tile>& Tiles, FlipDataHandle& FlipDataHandle, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;

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

		// search the wallkick translation required, if any
		uint8_t&& WallkickIndex = Zero;

		for (const WallKickAlignment& WallkickAlignment : WallkickAlignmentAtRotationIndex)
		{
			// check test case success

			static bool&& IsWallkickRequired = true;

			// process all matrix entries

			int8_t N = MinusOne;

			for (const auto& MatrixEntry : Matrix)
			{
				++N;
				if (MatrixEntry == MinusOne)
				{
					continue;
				}

				const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
				const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);

				// calculate new position

				uint16_t&& RealignmentOutput = static_cast<uint16_t>(Pivot + Col + (Row * Cols)
					+ RotationRealignmentValue
					+ FloorkickRealignmentValue
					);

				// check wallkick validity, OShape has none

				if (WallkickAlignment.IsValid())
				{
					RealignmentOutput += static_cast<uint16_t>(WallkickAlignment.x + (WallkickAlignment.y * Cols));
				}

				// check if the new position tile with wallkicks create overlaps

				const Tile& Tile = Tiles.at(RealignmentOutput);

				// check overlaps

				const bool&& IsTileOverlapping = (Tile.Attribute != TileAttributeEnum::Empty);

				IsWallkickRequired = IsTileOverlapping;

				if (IsTileOverlapping)
				{
					++WallkickIndex;
					break;
				}
			}

			if (!IsWallkickRequired)
			{
				break;
			}
		}

		const WallKickAlignment& WallkickAlignment = WallKickAlignmentContainer::TryGetWallKickAlignmentAtIndex(
			WallkickAlignmentContainer,
			TetrominoeRotationIndex,
			WallkickIndex);

		return static_cast<int8_t>((WallkickAlignment.y * Cols) + WallkickAlignment.x);
	}
	catch (const std::out_of_range& e)
	{
		SDLlogHelper::Print(PrefixErrorType::CollectionAccessFailed, "Tetrominoe", e);
	}

	return NULL;
}

int8_t Tetrominoe::GetRotationalAlignmentValueAtIndex(uint8_t Rows, uint8_t Cols)
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

	return static_cast<int8_t>((RotationalAlignment.y * Cols) + RotationalAlignment.x);
}

std::string Tetrominoe::GetTetrominoeWildcard() const
{
	static const std::unordered_map<TetrominoeShapeEnum, std::string>&& TetrominoeShapePair =
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

	const auto&& TetrominoeShapePairFound = TetrominoeShapePair.find(TetrominoeShape);

	return (TetrominoeShapePairFound != TetrominoeShapePair.end())
		? TetrominoeShapePairFound->second
		: std::string("Undefined");
}