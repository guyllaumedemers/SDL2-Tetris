#include "../include/LevelManager.h"
#include "../include/TileMap.h"
#include "../include/TetrominoeManager.h"
#include "../include/SDLTimerManager.h"
#include "../include/SDLlogHelper.h"

#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

#ifndef PERIODIC_UPDATE
#define PERIODIC_UPDATE 60.0
#endif

LevelManager::LevelManager()
{
	TileMapUniquePtr = std::make_shared<TileMap>();
	TetrominoeManagerUniquePtr = std::make_shared<TetrominoeManager>();
}

LevelManager::~LevelManager()
{
	TileMapUniquePtr = nullptr;
	TetrominoeManagerUniquePtr = nullptr;
}

void LevelManager::Initialize(std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	if (!TileMapUniquePtr)
	{
		return;
	}

	const uint8_t&& tempRows = 20;
	const uint8_t&& tempCols = 12;

	// intialize tilemap grid
	TileMapUniquePtr->Initialize(
		tempRows,
		tempCols,
		[&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); }
	);

	Subscribe();
}

void LevelManager::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!TileMapUniquePtr || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	// update tilemap
	TileMapUniquePtr->Update(
		TextureManagerPtrArg,
		SDLManagerPtrArg
	);
}

void LevelManager::Clear()
{
	UnSubscribe();
	if (!TileMapUniquePtr || !TetrominoeManagerUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Clear();
	TileMapUniquePtr->Clear();
}

void LevelManager::ResetGame(std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	Clear();
	Initialize(SetWindowEvent);
}

void LevelManager::PollKeyEvent(int8_t DirX, int8_t DirY) const
{
	if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Update(
		TileMapUniquePtr->GetTiles(),
		DirX,
		DirY,
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
}

void LevelManager::PollSpaceKeyEvent() const
{
	if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Flip(
		TileMapUniquePtr->GetTiles(),
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
}

void LevelManager::Subscribe()
{
	if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	// register function callback for generating tetrominoe when the active one lock
	TetrominoeManagerUniquePtr->GetLockDelayHandle().LockDelayCompletedDelegate = [&]()
	{
		if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, "LevelManager");
			return;
		}
		// update the active tetrominoe lock value
		TetrominoeManagerUniquePtr->OnLockDelayCompleted(
			TileMapUniquePtr->GetRows(),
			TileMapUniquePtr->GetCols()
		);
	};

	// wait for the above binding before starting registering to timer delegates
	Start();
}

void LevelManager::UnSubscribe()
{
	Stop();
}

void LevelManager::Start()
{
	static constexpr float&& One = 1.f;
	static constexpr float&& OneThousand = 1000.f;
	static const uint32_t&& Interval = std::floor((One / PERIODIC_UPDATE) * OneThousand);

	// register periodic udpate
	uint32_t(*PeriodUpdateFunctor)(uint32_t, void*) = [](uint32_t Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInTimerDelegate, "LevelManager");
			exit(EXIT_FAILURE);
		}
		LevelManagerPtr->OnPeriodicUpdate();
		return Interval;
	};

	PeriodicFunctorID = SDLTimerManager::CreateTimer(
		PeriodUpdateFunctor,
		Interval,
		const_cast<LevelManager*>(this)
	);

	if (!TetrominoeManagerUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->GetLockDelayHandle().LockDelayCompletedDelegate();
}

void LevelManager::Stop()
{
	SDLTimerManager::DestroyTimer(PeriodicFunctorID);
}

void LevelManager::OnPeriodicUpdate()
{
	if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	static constexpr int8_t&& OneDown = -1;
	static constexpr int8_t&& Idle = 0;

	// update tetrominoes
	TetrominoeManagerUniquePtr->Update(
		TileMapUniquePtr->GetTiles(),
		Idle,
		OneDown,
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
}