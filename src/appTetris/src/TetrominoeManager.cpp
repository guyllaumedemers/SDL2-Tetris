#include "../include/TetrominoeManager.h"
#include "../include/TileMap.h"

#ifndef INCLUDED_RANDOM
#define INCLUDED_RANDOM
#include <random>
#endif

void TetrominoeManager::Add()
{
	ActiveTetrominoe = GenerateRandomTetromioeShape();

	if (!ActiveTetrominoe)
	{
		return;
	}

	TetrominoePool.push_back(ActiveTetrominoe);
}

void TetrominoeManager::Remove()
{
}

std::unique_ptr<Tetrominoe> TetrominoeManager::GenerateRandomTetromioeShape() const
{
	std::random_device Seed;
	std::mt19937 RandomGenerator(Seed());
	std::uniform_int_distribution<int> UniformDistribution(TetrominoeShapeEnum::TShape, TetrominoeShapeEnum::SShape);
	return std::make_unique<Tetrominoe>(static_cast<TetrominoeShapeEnum>(UniformDistribution(RandomGenerator)));
}

void TetrominoeManager::Initialize(TileMap* TileMapPtrArg)
{
	GenerateRandomTetrominoeEvent = [&]()
	{
		Add();
	};

	DelCheckRowCompletionEvent = [&](Tetrominoe* TetrominoePtrArg)
	{
		if (!TileMapPtrArg || !TetrominoePtrArg)
		{
			return;
		}

		for (const auto& TetrominoeEntryIndex : TetrominoePtrArg->GetTetrominoeIndices())
		{
			TileMapPtrArg->CheckRowCompletion(TetrominoeEntryIndex);
		}
	};
}

void TetrominoeManager::Update(TileMap* TileMapPtrArg, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
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
	DelCheckRowCompletionEvent = nullptr;

	for (auto& TetrominoeSharedPtr : TetrominoePool)
	{
		TetrominoeSharedPtr.reset();
	}

	ActiveTetrominoe.reset();
}

void TetrominoeManager::Flip() const
{
	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->Flip();
}