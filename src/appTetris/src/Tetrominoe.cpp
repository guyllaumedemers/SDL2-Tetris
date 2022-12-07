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
		// squared matrix 3 x 3

		//	1, 2, 3				7, 4, 1
		//  4, 5, 6		=>		8, 5, 2
		//	7, 8, 9				9, 6, 3

		// initial permutation require to flip Rows & Columns

		//	1, 4, 7				and then...	Row flip			7, 4, 1
		//	2, 5, 8		=>								=>		8, 5, 2
		//	3, 6, 9												9, 6, 3

		// what happen in a 2 x 3 matrix?

		//	1, 2				5, 3, 1
		//	3, 4		=>		6, 4, 2
		//	5, 6

		// does squared matrix permutation be applied here? else can I fake a 4 x 4 matrix?

		//	1, 2, 3, 4			13, 9, 5, 1				
		//	5, 6, 7, 8	=>		14,10, 6, 2				
		//	9,10,11,12			15,11, 7, 3
		// 13,14,15,16			16,12, 8, 4

		// initial permutation require to flip Rows & Columns

		//	1, 5, 9,13
		//	2, 6,10,14
		//	3, 7,11,15
		//	4, 8,12,16

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
