#include "../include/TetrominoeManager.h"
#include "../include/Tetrominoe.h"
#include "../include/SDLTimerManager.h"
#include "../include/SDLlogHelper.h"

#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#endif

#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

#ifndef STDEXCEPT_H
#define STDEXCEPT_H
#include <stdexcept>
#endif

#ifndef LOCKDELAY
#define LOCKDELAY 30.0
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
	Reset();
}

void TetrominoeManager::OnLockDelayCompleted(uint8_t Rows, uint8_t Cols)
{
	// lock the current tetrominoe
	Lock();
	// generate a new entry
	Add(Rows, Cols);
	// reset lock delay for new entry
	Reset();
}

void TetrominoeManager::Flip(const std::vector<Tile>& Tiles, uint8_t Rows, uint8_t Cols)
{
	std::lock_guard<std::mutex> lock(ActiveTetrominoeMutex);

	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->FlipMatrix(const_cast<std::vector<Tile>&>(Tiles), Rows, Cols);
	Reset();
}

void TetrominoeManager::Clear()
{
	for (auto& TetrominoeSharedPtr : TetrominoePool)
	{
		TetrominoeSharedPtr.reset();
	}

	ActiveTetrominoe.reset();
}

void TetrominoeManager::Reset()
{
	Stop();
	Start();
}

void TetrominoeManager::Start()
{
	static constexpr float&& One = 1.f;
	static constexpr float&& OneThousand = 1000.f;
	static const uint32_t&& Interval = std::floor((One / LOCKDELAY) * OneThousand);

	// register new tetrominoe generation
	uint32_t(*StartLockDelayCountdownFunctor)(uint32_t, void*) = [](uint32_t Interval, void* Param)
	{
		TetrominoeManager* const TetrominoeManagerPtr = static_cast<TetrominoeManager*>(Param);
		if (!TetrominoeManagerPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInTimerDelegate, "TetrominoeManager");
			exit(EXIT_FAILURE);
		}
		TetrominoeManagerPtr->GetLockDelayHandle().LockDelayCompletedDelegate();
		return static_cast<uint32_t>(NULL);
	};

	TetrominoeLockHandle.LockID = SDLTimerManager::CreateTimer(
		StartLockDelayCountdownFunctor,
		Interval,
		const_cast<TetrominoeManager*>(this)
	);
}

void TetrominoeManager::Stop()
{
	SDLTimerManager::DestroyTimer(TetrominoeLockHandle.LockID);
}

void TetrominoeManager::Add(uint8_t Rows, uint8_t Cols)
{
	ActiveTetrominoe = GenerateRandomTetromioe(Rows, Cols);
	if (!ActiveTetrominoe)
	{
		return;
	}

	ActiveTetrominoe->Align();
	TetrominoePool.push_back(ActiveTetrominoe);
}

void TetrominoeManager::Remove(std::shared_ptr<Tetrominoe> TetrominoePtrArg)
{
	// create a system that turn the tetrominoe indices to -1, lookup for a tetrominoe
	// where all indices are -1 and delete it ONLY when this case is TRUE.
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

void TetrominoeManager::Lock()
{
	if (!ActiveTetrominoe)
	{
		return;
	}
	ActiveTetrominoe->SetIsLocked();
}
