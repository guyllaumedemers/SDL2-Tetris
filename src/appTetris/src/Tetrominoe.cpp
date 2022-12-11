#include "../include/Tetrominoe.h"
#include "../include/Tile.h"

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

Tetrominoe::Tetrominoe(TetrominoeShapeEnum TetrominoeEnum, uint8_t Rows, uint8_t Cols)
{
	if (!TetrominoeEnum /*TetrominoeShapeEnum::None == 0*/)
	{
		return;
	}

	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;
	static constexpr uint8_t&& Two = 2;
	static constexpr uint8_t&& Three = 3;

	switch (TetrominoeEnum)
	{
	case None:
		TetrominoeEntryIndices = { Zero, Zero, Zero, Zero };
		break;
	case TShape:
		TetrominoeEntryIndices = { Zero, One, Two, static_cast<uint8_t>(One + (One * Cols)) };
		break;
	case LShape:
		TetrominoeEntryIndices = { Zero, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(Two * Cols), static_cast<uint8_t>(One + (Two * Cols)) };
		break;
	case ZShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two + (One * Cols)) };
		break;
	case OShape:
		TetrominoeEntryIndices = { Zero, One, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	case IShape:
		TetrominoeEntryIndices = { Zero, One, Two, Three };
		break;
	case JShape:
		TetrominoeEntryIndices = { One, static_cast<uint8_t>(One + (One * Cols)), static_cast<uint8_t>(Two * Cols), static_cast<uint8_t>(One + (Two * Cols)) };
		break;
	case SShape:
		TetrominoeEntryIndices = { One, Two, static_cast<uint8_t>(One * Cols), static_cast<uint8_t>(One + (One * Cols)) };
		break;
	default:
		break;
	}
	TetrominoeShape = TetrominoeEnum;
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

		uint8_t&& MinRow = 255;
		uint8_t&& MinCol = 255;

		for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			const uint8_t&& Col = static_cast<uint8_t>(TetrominoeEntryIndex % Cols);
			const uint8_t&& Row = static_cast<uint8_t>(TetrominoeEntryIndex / Cols);

			if (Col < MinCol) MinCol = Col;
			if (Row < MinRow) MinRow = Row;
		}

		const uint16_t&& Pivot = (MinRow * Cols) + MinCol;

		const size_t&& NMatrix = TetrominoeEntryIndices.size();
		const size_t&& Zero = 0;
		const size_t&& One = 1;
		const size_t&& Two = 2;
		const int8_t&& MinusOne = -1;

		std::vector<int16_t> Matrix(16, -1);

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
			const uint8_t&& ColPrime = static_cast<uint8_t>((NMatrix - One - (N % NMatrix)) + (Row * NMatrix));

			// Temp
			int16_t Temp = Matrix.at((Row * NMatrix) + Col);

			// Front
			Matrix.at((Row * NMatrix) + Col) = Matrix.at((Row * NMatrix) + ColPrime);

			// Back
			Matrix.at((Row * NMatrix) + ColPrime) = Temp;
		}

		// update array indices

		size_t&& Begin = 0;

		for (size_t N = Zero; N < (NMatrix * NMatrix); ++N)
		{
			const int16_t& Index = Matrix.at(N);
			if ((Begin < NMatrix) && (Index != MinusOne))
			{
				TetrominoeEntryIndices.at(Begin++) = Index;
			}
		}
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
