#include "../include/TetrominoeManager.h"
#include "../include/TileMap.h"

#ifndef INCLUDED_RANDOM
#define INCLUDED_RANDOM
#include <random>
#endif

#ifndef INCLUDED_SDL_LOG
#define INCLUDED_SDL_LOG
#include <SDL_log.h>
#endif

void TetrominoeManager::Add(uint8_t Rows, uint8_t Cols)
{
	ActiveTetrominoe = GenerateRandomTetromioeShape(Rows, Cols);

	if (!ActiveTetrominoe)
	{
		return;
	}

	// initialize Tetrominoe spawn point
	ActiveTetrominoe->Align();
	// append the pool
	TetrominoePool.push_back(ActiveTetrominoe);
}

void TetrominoeManager::Remove()
{
	// do something
}

std::unique_ptr<Tetrominoe> TetrominoeManager::GenerateRandomTetromioeShape(uint8_t Rows, uint8_t Cols) const
{
	std::random_device Seed;
	std::mt19937 RandomGenerator(Seed());
	std::uniform_int_distribution<int> UniformDistribution(TetrominoeShapeEnum::TShape, TetrominoeShapeEnum::SShape);
	return std::make_unique<Tetrominoe>(static_cast<TetrominoeShapeEnum>(UniformDistribution(RandomGenerator)), Rows, Cols);
}

void TetrominoeManager::ClearTetrominoesOnRow(TileMap* const TileMapPtrArg, size_t TetrominoeIndex)
{
	if (!TileMapPtrArg)
	{
		return;
	}

	const uint8_t& Rows = TileMapPtrArg->GetRows();
	const uint8_t& Cols = TileMapPtrArg->GetCols();

	const uint8_t&& TargetRow = static_cast<uint8_t>(TetrominoeIndex / Cols);
	const uint8_t&& Zero = 0;
	const uint8_t&& One = 1;

	try
	{
		for (auto& TetrominoeSharedPtr : TetrominoePool)
		{
			Tetrominoe* const TetrominoePtr = TetrominoeSharedPtr.get();
			if (!TetrominoePtr)
			{
				continue;
			}

			const std::vector<uint16_t>& TetrominoeEntryIndicies = TetrominoePtr->GetTetrominoeIndices();
			for (size_t Index = TetrominoeEntryIndicies.size() - One; Index >= Zero; --Index)
			{
				const uint8_t&& TetrominoeEntryIndexRow = static_cast<uint8_t>(TetrominoeEntryIndicies.at(Index) / Cols);
				if (TetrominoeEntryIndexRow != TargetRow)
				{
					continue;
				}
				const_cast<std::vector<uint16_t>&>(TetrominoeEntryIndicies).erase(std::prev(TetrominoeEntryIndicies.end()));
			}
		}
	}
	catch (const std::out_of_range& e)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TRY CATCH FAILED IN CLEAR ROW FUNCTION! %s", e.what());
	}
}

void TetrominoeManager::RealignTetrominoes(TileMap* const TileMapPtrArg) const
{
	if (!TileMapPtrArg)
	{
		return;
	}

	const uint8_t& Rows = TileMapPtrArg->GetRows();
	const uint8_t& Cols = TileMapPtrArg->GetCols();

	for (const auto& TetrominoeSharedPtr : TetrominoePool)
	{
		Tetrominoe* const TetrominoePtr = TetrominoeSharedPtr.get();
		if (!TetrominoePtr)
		{
			continue;
		}

		TetrominoePtr->Realign(TileMapPtrArg->GetTiles(), Rows, Cols);
	}
}

void TetrominoeManager::Initialize(TileMap* const TileMapPtrArg)
{
	GenerateRandomTetrominoeEvent = [&]()
	{
		if (!TileMapPtrArg)
		{
			return;
		}

		const uint8_t& Rows = TileMapPtrArg->GetRows();
		const uint8_t& Cols = TileMapPtrArg->GetCols();
		Add(Rows, Cols);
	};

	CheckRowCompletionEvent = [&](Tetrominoe* const TetrominoePtrArg)
	{
		if (!TileMapPtrArg || !TetrominoePtrArg)
		{
			return;
		}

		for (const auto& TetrominoeEntryIndex : TetrominoePtrArg->GetTetrominoeIndices())
		{
			const bool&& bIsRowComplete = TileMapPtrArg->CheckRowCompletion(TetrominoeEntryIndex);
			if (!bIsRowComplete)
			{
				continue;
			}

			ClearTetrominoesOnRow(TileMapPtrArg, TetrominoeEntryIndex);
		}

		RealignTetrominoes(TileMapPtrArg);
	};
}

void TetrominoeManager::Update(TileMap* const TileMapPtrArg, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols) const
{
	if (!TileMapPtrArg)
	{
		return;
	}

	for (const auto& TetrominoeUniquePtr : TetrominoePool)
	{
		Tetrominoe* const TetrominoePtr = TetrominoeUniquePtr.get();
		if (!TetrominoePtr)
		{
			continue;
		}

		if (TetrominoePtr->IsLocked())
		{
			continue;
		}

		const bool&& IsMoveInBound = TetrominoePtr->IsMoveInBound(DirX, DirY, Rows, Cols);
		if (!IsMoveInBound)
		{
			continue;
		}

		const bool&& IsMoveOverlappingExistingTile = TetrominoePtr->IsMoveOverlappingExistingTile(TileMapPtrArg->GetTiles(), DirX, DirY, Rows, Cols);
		if (IsMoveOverlappingExistingTile)
		{
			continue;
		}

		TetrominoePtr->Update(const_cast<std::vector<Tile>&>(TileMapPtrArg->GetTiles()), DirX, DirY, Rows, Cols);
	}
}

void TetrominoeManager::Clear()
{
	GenerateRandomTetrominoeEvent = nullptr;
	CheckRowCompletionEvent = nullptr;

	for (auto& TetrominoeSharedPtr : TetrominoePool)
	{
		TetrominoeSharedPtr.reset();
	}

	ActiveTetrominoe.reset();
}

void TetrominoeManager::Flip(uint8_t Rows, uint8_t Cols) const
{
	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->FlipClockwise(Rows, Cols);
}