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
#define PERIODIC_UPDATE 30.0
#endif

LevelManager::LevelManager()
{
	TileMapSharedPtr = std::make_shared<TileMap>();
	TetrominoeManagerSharedPtr = std::make_shared<TetrominoeManager>();
}

LevelManager::~LevelManager()
{
	TileMapSharedPtr = nullptr;
	TetrominoeManagerSharedPtr = nullptr;
}

void LevelManager::Initialize(std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	if (!TileMapSharedPtr)
	{
		return;
	}

	const uint8_t& tempRows = 20;
	const uint8_t& tempCols = 12;

	// intialize tilemap grid
	TileMapSharedPtr->Initialize(
		tempRows,
		tempCols,
		[&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); }
	);

	Subscribe();
}

void LevelManager::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!TileMapSharedPtr || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	// update tilemap
	TileMapSharedPtr->Update(
		TextureManagerPtrArg,
		SDLManagerPtrArg
	);
}

void LevelManager::Clear()
{
	UnSubscribe();
	if (!TileMapSharedPtr || !TetrominoeManagerSharedPtr)
	{
		return;
	}

	TetrominoeManagerSharedPtr->Clear();
	TileMapSharedPtr->Clear();
}

void LevelManager::ResetGame(std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	Clear();
	Initialize(SetWindowEvent);
}

void LevelManager::PollKeyEvent(int8_t DirX, int8_t DirY) const
{
	if (!TetrominoeManagerSharedPtr || !TileMapSharedPtr)
	{
		return;
	}

	TetrominoeManagerSharedPtr->Update(
		TileMapSharedPtr->GetTiles(),
		DirX,
		DirY,
		TileMapSharedPtr->GetRows(),
		TileMapSharedPtr->GetCols()
	);
}

void LevelManager::PollSpaceKeyEvent() const
{
	if (!TetrominoeManagerSharedPtr || !TileMapSharedPtr)
	{
		return;
	}

	TetrominoeManagerSharedPtr->Flip(
		TileMapSharedPtr->GetTiles(),
		TileMapSharedPtr->GetRows(),
		TileMapSharedPtr->GetCols()
	);
}

void LevelManager::Subscribe()
{
	if (!TetrominoeManagerSharedPtr || !TileMapSharedPtr)
	{
		return;
	}

	// register function callback for generating tetrominoe when the active one lock
	TetrominoeManagerSharedPtr->GetLockDelayHandle().LockDelayCompletedDelegate = [&]()
	{
		if (!TetrominoeManagerSharedPtr || !TileMapSharedPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInDelegate, "LevelManager");
			return;
		}
		// update the active tetrominoe lock value
		TetrominoeManagerSharedPtr->OnLockDelayCompleted(
			TileMapSharedPtr->GetRows(),
			TileMapSharedPtr->GetCols()
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
	static const float& One = 1.f;
	static const float& OneThousand = 1000.f;
	static const uint32_t& Interval = std::floor((One / PERIODIC_UPDATE) * OneThousand);

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

	if (!TetrominoeManagerSharedPtr)
	{
		return;
	}

	TetrominoeManagerSharedPtr->GetLockDelayHandle().LockDelayCompletedDelegate();
}

void LevelManager::Stop()
{
	SDLTimerManager::DestroyTimer(PeriodicFunctorID);
}

void LevelManager::OnPeriodicUpdate()
{
	if (!TetrominoeManagerSharedPtr || !TileMapSharedPtr)
	{
		return;
	}

	static const int8_t& OneDown = -1;
	static const int8_t& Idle = 0;

	// update tetrominoes
	TetrominoeManagerSharedPtr->Update(
		TileMapSharedPtr->GetTiles(),
		Idle,
		OneDown,
		TileMapSharedPtr->GetRows(),
		TileMapSharedPtr->GetCols()
	);
}