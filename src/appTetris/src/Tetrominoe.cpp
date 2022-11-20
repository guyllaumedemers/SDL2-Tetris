#include "../include/Tetrominoe.h"
#include "../include/Tile.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
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
		const bool&& CanMoveRow = ((TetrominoeEntryIndex / Cols) + DirY) >= Zero && ((TetrominoeEntryIndex / Cols) + DirY) < Cols;

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

	const uint8_t&& JumpValue = static_cast<uint8_t>(DirX + (std::abs(DirY) * Cols));

	try
	{
		for (const auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
		{
			const Tile& Tile = Tiles.at(TetrominoeEntryIndex + JumpValue);
			if (Tile.Attribute != TileAttributeEnum::Empty)
			{
				return IsMoveOverlappingExistingTile;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		// print error message
	}
	return !IsMoveOverlappingExistingTile;
}

void Tetrominoe::Update(std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	if (Tiles.empty())
	{
		return;
	}

	const uint8_t&& JumpValue = static_cast<uint8_t>(DirX + (std::abs(DirY) * Cols));
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

			{
				Tile& NextTile = Tiles.at(TetrominoeEntryIndex);

				NextTile.Attribute = TileAttributeEnum::Filled;
				NextTile.Wildcard = Wildcard;
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		// print error message
	}
}

void Tetrominoe::FlipClockwise(uint8_t Rows, uint8_t Cols)
{
	if (IsLocked())
	{
		return;
	}

	for (auto& TetrominoeEntryIndex : TetrominoeEntryIndices)
	{
		// need a bit of thinking here to do it right.

		// shapes are different, bounds are different and rotation behaviour are different
	}
}

void Tetrominoe::Realign(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	static constexpr int8_t&& Zero = 0;
	static constexpr int8_t&& MinueOne = -1;

	while (IsMoveInBound(Zero, MinueOne, Rows, Cols) && !IsMoveOverlappingExistingTile(Tiles, Zero, MinueOne, Rows, Cols))
	{
		Update(const_cast<std::vector<Tile>&>(Tiles), Zero, MinueOne, Rows, Cols);
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
