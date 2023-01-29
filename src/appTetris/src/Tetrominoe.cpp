#include "../include/Tetrominoe.h"
#include "../include/Tile.h"
#include "../include/TetrominoeWallKickHelper.h"
#include "../include/TetrominoeRotationRealignmentHelper.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

#ifndef INCLUDED_RANGE
#define INCLUDED_RANGE
#include <ranges>
#endif

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
#endif

#ifndef INCLUDED_ALGORITHM
#define INCLUDED_ALGORITHM
#include <algorithm>
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
}

bool Tetrominoe::IsMoveInBound(int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	static constexpr bool&& IsMoveInBound = true;
	static constexpr uint8_t&& Zero = 0;

	for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		const bool&& CanMoveCol = ((TetrominoeEntryIndex % Cols) + DirX) >= Zero && ((TetrominoeEntryIndex % Cols) + DirX) < Cols;
		const bool&& CanMoveRow = ((TetrominoeEntryIndex / Cols) + std::abs(DirY)) >= Zero && ((TetrominoeEntryIndex / Cols) + std::abs(DirY)) < Rows;

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

	const int8_t&& JumpValue = static_cast<int8_t>(DirX + (std::abs(DirY) * Cols));

	try
	{
		for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			const bool&& IsJumpIndexOverlappingItsOwnIndex = std::find(TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(),
				TetrominoeEntryIndex + JumpValue) != TetrominoeEntryIndices.end();

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
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN OVERLAPPING FUNCTION! %s", e.what());
	}
	return !IsMoveOverlappingExistingTile;
}

void Tetrominoe::Update(std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	if (Tiles.empty())
	{
		return;
	}

	const int8_t&& JumpValue = static_cast<int8_t>(DirX + (std::abs(DirY) * Cols));
	const std::string&& Wildcard = GetTetrominoeWildcard();

	try
	{
		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			{
				Tile& PreviousTile = Tiles.at(TetrominoeEntryIndex);

				PreviousTile.Attribute = TileAttributeEnum::Empty;
				PreviousTile.Wildcard = std::string("Undefined");
			}

			TetrominoeEntryIndex += JumpValue;
		}

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			{
				Tile& NextTile = Tiles.at(TetrominoeEntryIndex);

				NextTile.Attribute = TileAttributeEnum::Filled;
				NextTile.Wildcard = Wildcard;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN UPDATE FUNCTION! %s", e.what());
	}
}

void Tetrominoe::FlipMatrix(std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	if (IsLocked())
	{
		return;
	}

	try
	{
		// find pivot point of the matrix

		const uint16_t& Pivot = GetFlipMatrixPivot(Rows, Cols);

		// NMatrix grid size

		static const size_t&& NMatrix = TetrominoeEntryIndices.size();

		// generate matrix using tetrominoe entry indicies

		const std::vector<int16_t>& Matrix = GenerateFlipMatrix(Pivot, NMatrix, Rows, Cols);

		// invalide current tetrominoe entry indicies in tilemap

		InvalidateTetrominoeIndicies(Tiles);

		// increase Rotational Index

		UpdateTetrominoeRotationIndex();

		// calculate rotational alignment translation to apply

		const int8_t&& RotationReAlignmentValue = GetRotationalAlignmentValueAtIndex(Rows, Cols);

		// calculate floor kick alignment translation to apply

		const int8_t&& FloorkickReAlignmentValue = GetFloorKickAlignmentValueAtRotation(Tiles, Matrix, Pivot, RotationReAlignmentValue, NMatrix, Rows, Cols);

		// search wallkick alignment index

		const int8_t&& WallkickReAlignmentValue = GetWallkickAlignmentValueAtIndex(Tiles, Matrix, Pivot, RotationReAlignmentValue, FloorkickReAlignmentValue, NMatrix, Rows, Cols);

		// update tetrominoe entry indices with proper translated values

		UpdateTetrominoeEntryIndicies(Matrix, Pivot, RotationReAlignmentValue, FloorkickReAlignmentValue, WallkickReAlignmentValue, NMatrix, Rows, Cols);

		// revalide current tetrominoe entry indicies in tilemap

		RevalidateTetrominoeIndicies(Tiles);
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN FLIP FUNCTION FUNCTION! %s", e.what());
	}
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
	return (TetrominoeShapePairFound != TetrominoeShapePair.end()) ? TetrominoeShapePairFound->second : std::string("Undefined");
}

void Tetrominoe::InvalidateTetrominoeIndicies(std::vector<Tile>& Tiles)
{
	static constexpr TileAttributeEnum&& EmptyEnum = TileAttributeEnum::Empty;
	static const std::string&& UndefinedString = std::string("Undefined");

	// clear tiles entry for the active indices

	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		Tile& Tile = Tiles.at(TetrominoeEntryIndex);
		Tile.Attribute = EmptyEnum;
		Tile.Wildcard = UndefinedString;
	}
}

void Tetrominoe::RevalidateTetrominoeIndicies(std::vector<Tile>& Tiles)
{
	static constexpr TileAttributeEnum&& FilledEnum = TileAttributeEnum::Filled;
	static std::string&& DefinedString = GetTetrominoeWildcard();

	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		Tile& Tile = Tiles.at(TetrominoeEntryIndex);
		Tile.Attribute = FilledEnum;
		Tile.Wildcard = DefinedString;
	}
}

void Tetrominoe::UpdateTetrominoeRotationIndex()
{
	static constexpr size_t&& One = 1;
	static constexpr size_t&& Four = 4;
	SetTetrominoeRotationIndex((GetTetrominoeRotationIndex() + One) % Four);
}

void Tetrominoe::UpdateTetrominoeEntryIndicies(const std::vector<int16_t>& Matrix, uint16_t Pivot, int8_t RotationReAlignmentValue, int8_t FloorkickReAlignmentValue,
	int8_t WallkickReAlignmentValue, size_t NMatrix, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;

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
				+ RotationReAlignmentValue
				+ FloorkickReAlignmentValue
				+ WallkickReAlignmentValue
				);

			// realign

			TetrominoeEntryIndices.at(Begin++) = RealignmentOutput;
		}
	}
}

int8_t Tetrominoe::GetFloorKickAlignmentValueAtRotation(const std::vector<Tile>& Tiles, const std::vector<int16_t>& Matrix, uint16_t Pivot,
	int8_t RotationReAlignmentValue, size_t NMatrix, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;
	static constexpr int8_t&& MinusTwo = -2;

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

		uint16_t&& RealignmentOutput = static_cast<uint16_t>(Pivot + Col + (Row * Cols) + RotationReAlignmentValue);

		// check if the new position tile with wallkicks create overlaps

		const Tile& Tile = Tiles.at(RealignmentOutput);

		// check overlaps

		const bool&& IsTileOverlapping =
			(Tile.Attribute != TileAttributeEnum::Empty);

		if (IsTileOverlapping)
		{
			const bool&& CanPerformFloorKick =
				static_cast<bool>(GetTetrominoeShape() & ~TetrominoeShapeEnum::OShape);

			if (CanPerformFloorKick)
			{
				return static_cast<bool>(GetTetrominoeShape() & ~TetrominoeShapeEnum::IShape) ? (MinusOne * Cols) : (MinusTwo * Cols);
			}
		}
	}

	return NULL;
}

int8_t Tetrominoe::GetWallkickAlignmentValueAtIndex(const std::vector<Tile>& Tiles, const std::vector<int16_t>& Matrix, uint16_t Pivot,
	int8_t RotationReAlignmentValue, int8_t FloorkickReAlignmentValue, size_t NMatrix, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinusOne = -1;

	uint8_t&& WallkickIndex = Zero;

	// --- typedef
	using WallKickAlignment = WallKickAlignmentContainer::WallKickAlignment;
	// ---

	const WallKickAlignmentContainer& WallkickContainer = TetrominoeWallKickHelper::Get()->TryGetWallKickAlignmentContainer(this);

	const uint8_t& RotationIndex = GetTetrominoeRotationIndex();
	const std::vector<WallKickAlignment>& WallkickAlignmentAtRotationIndex = WallkickContainer.TryGetWallkickAlignmentAtRotationIndex(RotationIndex);

	// search the wallkick translation required, if any

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
				+ RotationReAlignmentValue
				+ FloorkickReAlignmentValue
				);

			// check wallkick validity, OShape has none

			if (WallkickAlignment.IsValid())
			{
				RealignmentOutput += static_cast<uint16_t>(WallkickAlignment.x + (WallkickAlignment.y * Cols));
			}

			// check if the new position tile with wallkicks create overlaps

			const Tile& Tile = Tiles.at(RealignmentOutput);

			// check overlaps

			const bool&& IsTileOverlapping =
				(Tile.Attribute != TileAttributeEnum::Empty);

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

	const WallKickAlignment& WallkickAlignment = WallkickAlignmentAtRotationIndex.at(WallkickIndex);
	return static_cast<int8_t>((WallkickAlignment.y * Cols) + WallkickAlignment.x);
}

int8_t Tetrominoe::GetRotationalAlignmentValueAtIndex(uint8_t Rows, uint8_t Cols)
{
	// --- typedef
	using RotationRealignment = RotationalAlignmentContainer::RotationalAlignment;
	// ---

	const RotationalAlignmentContainer& RotationalAlignmentContainer = TetrominoeRotationRealignmentHelper::Get()->TryGetRotationAlignmentContainer(this);

	const uint8_t& RotationIndex = GetTetrominoeRotationIndex();
	const RotationRealignment& RotationalAlignment = RotationalAlignmentContainer.TryGetRotationAlignmentAtIndex(RotationIndex);

	if (!RotationalAlignment.IsValid())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: BAD ROTATION ALIGNMENT VALUE! CANNOT PERFORM FLIP");
		exit(NULL);
	}

	return static_cast<int8_t>((RotationalAlignment.y * Cols) + RotationalAlignment.x);
}

std::vector<int16_t> Tetrominoe::GenerateFlipMatrix(uint16_t Pivot, size_t NMatrix, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& One = 1;
	static constexpr int8_t&& Two = 2;
	static constexpr int8_t&& MinusOne = -1;

	std::vector<int16_t> Matrix(NMatrix * NMatrix, MinusOne);

	// flip matrix col, row

	for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
	{
		const uint8_t&& Col = static_cast<uint8_t>(N % NMatrix);
		const uint8_t&& Row = static_cast<uint8_t>(N / NMatrix);

		if (Col < Row)
		{
			continue;
		}

		uint16_t&& ColIndex = static_cast<uint16_t>(Pivot + Col + (Row * Cols));
		uint16_t&& RowIndex = static_cast<uint16_t>(Pivot + Row + (Col * Cols));

		const bool&& IsColIndexInArray = std::find(TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(),
			ColIndex) != TetrominoeEntryIndices.end();

		const bool&& IsRowIndexInArray = std::find(TetrominoeEntryIndices.begin(), TetrominoeEntryIndices.end(),
			RowIndex) != TetrominoeEntryIndices.end();

		// Col Permutation
		Matrix.at(Col + (Row * NMatrix)) = IsRowIndexInArray ? static_cast<int16_t>(RowIndex) : static_cast<int16_t>(MinusOne);

		// Row Permutation
		Matrix.at(Row + (Col * NMatrix)) = IsColIndexInArray ? static_cast<int16_t>(ColIndex) : static_cast<int16_t>(MinusOne);
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

	return Matrix;
}

uint16_t Tetrominoe::GetFlipMatrixPivot(uint8_t Rows, uint8_t Cols)
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
