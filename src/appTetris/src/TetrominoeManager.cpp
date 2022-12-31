#include "../include/TetrominoeManager.h"

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION
#include <stdexcept>
#endif

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
	static constexpr int&& One = 1;
	static constexpr int&& Two = 2;
	static const int&& B1 = static_cast<int>(std::log2(static_cast<int>(TetrominoeShapeEnum::TShape)) + One);
	static const int&& B2 = static_cast<int>(std::log2(static_cast<int>(TetrominoeShapeEnum::SShape)) + One);
	std::random_device Seed;
	std::mt19937 RandomGenerator(Seed());
	std::uniform_int_distribution<int> UniformDistribution(B1, B2);
	return std::make_unique<Tetrominoe>(static_cast<TetrominoeShapeEnum>(static_cast<int>(std::pow(Two, UniformDistribution(RandomGenerator) - One))), Rows, Cols);
}

void TetrominoeManager::ClearTetrominoesOnRow(uint8_t Rows, uint8_t Cols, size_t TetrominoeIndex)
{
	static constexpr uint8_t&& Zero = 0;
	static constexpr uint8_t&& One = 1;
	const uint8_t&& TargetRow = static_cast<uint8_t>(TetrominoeIndex / Cols);

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

void TetrominoeManager::RealignTetrominoes(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols) const
{
	for (const auto& TetrominoeSharedPtr : TetrominoePool)
	{
		Tetrominoe* const TetrominoePtr = TetrominoeSharedPtr.get();
		if (!TetrominoePtr)
		{
			continue;
		}

		TetrominoePtr->Realign(const_cast<std::vector<Tile>&>(Tiles), Rows, Cols);
	}
}

void TetrominoeManager::Initialize(const std::vector<Tile>& Tiles, const uint8_t& Rows, const uint8_t& Cols, std::function<bool(uint16_t)> RowCompletionCallback)
{
	GenerateRandomTetrominoeEvent = [&]()
	{
		Add(Rows, Cols);
	};

	CheckRowCompletionEvent = [&](Tetrominoe* const TetrominoePtrArg)
	{
		if (!TetrominoePtrArg)
		{
			return;
		}

		for (const auto& TetrominoeEntryIndex : TetrominoePtrArg->GetTetrominoeIndices())
		{
			const bool&& IsRowComplete = RowCompletionCallback(TetrominoeEntryIndex);
			if (!IsRowComplete)
			{
				continue;
			}

			ClearTetrominoesOnRow(Rows, Cols, TetrominoeEntryIndex);
		}

		RealignTetrominoes(Tiles, Rows, Cols);
	};
}

void TetrominoeManager::Update(const std::vector<Tile>& Tiles, int8_t DirX, int8_t DirY, uint8_t Rows, uint8_t Cols)
{
	std::lock_guard<std::mutex> lock(ActiveTetrominoeMutex);

	Tetrominoe* const TetrominoePtr = ActiveTetrominoe.get();
	if (!TetrominoePtr)
	{
		return;
	}

	if (TetrominoePtr->IsLocked())
	{
		return;
	}

	const bool&& IsMoveInbound = TetrominoePtr->IsMoveInBound(DirX, DirY, Rows, Cols);
	if (!IsMoveInbound)
	{
		return;
	}

	const bool&& IsMoveOverlappingExistingTile = TetrominoePtr->IsMoveOverlappingExistingTile(Tiles, DirX, DirY, Rows, Cols);
	if (IsMoveOverlappingExistingTile)
	{
		return;
	}

	TetrominoePtr->Update(const_cast<std::vector<Tile>&>(Tiles), DirX, DirY, Rows, Cols);
}

void TetrominoeManager::Flip(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	std::lock_guard<std::mutex> lock(ActiveTetrominoeMutex);

	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->FlipMatrix(const_cast<std::vector<Tile>&>(Tiles), Rows, Cols);
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