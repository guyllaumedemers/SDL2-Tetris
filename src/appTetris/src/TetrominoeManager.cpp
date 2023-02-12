#include "../include/TetrominoeManager.h"
#include "../include/Tetrominoe.h"
#include "../include/SDLlogHelper.h"

#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

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
	for (auto& TetrominoeSharedPtr : TetrominoePool)
	{
		TetrominoeSharedPtr.reset();
	}

	ActiveTetrominoe.reset();
}

std::shared_ptr<Tetrominoe> TetrominoeManager::GenerateRandomTetromioe(uint8_t Rows, uint8_t Cols) const
{
	static constexpr int&& One = 1;
	static constexpr int&& Two = 2;
	static const int&& B1 = static_cast<int>(std::log2(static_cast<int>(TetrominoeShapeEnum::TShape)) + One);
	static const int&& B2 = static_cast<int>(std::log2(static_cast<int>(TetrominoeShapeEnum::SShape)) + One);
	std::random_device Seed;
	std::mt19937 RandomGenerator(Seed());
	std::uniform_int_distribution<int> UniformDistribution(B1, B2);
	return std::make_shared<Tetrominoe>(static_cast<TetrominoeShapeEnum>(static_cast<int>(std::pow(Two, UniformDistribution(RandomGenerator) - One))), Rows, Cols);
}

void TetrominoeManager::Subscribe()
{
}

void TetrominoeManager::UnSubscribe()
{
}