#include "../include/LevelManager.h"
#include "../include/TileMap.h"
#include "../include/LockDelayManager.h"
#include "../include/TetrominoeManager.h"
#include "../include/SDLTimerManager.h"
#include "../include/SDLlogHelper.h"

#ifndef LOCK_DELAY
#define LOCK_DELAY 30.0
#endif

#ifndef PERIODIC_UPDATE
#define PERIODIC_UPDATE 60.0
#endif

LevelManager::LevelManager()
{
	TileMapUniquePtr = std::make_shared<TileMap>();
	LockDelayManagerUniquePtr = std::make_shared<LockDelayManager>();
	TetrominoeManagerUniquePtr = std::make_shared<TetrominoeManager>();
}

LevelManager::~LevelManager()
{
	TileMapUniquePtr = nullptr;
	LockDelayManagerUniquePtr = nullptr;
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

void LevelManager::Reset(std::function<void(uint16_t, uint16_t)> SetWindowEvent)
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

	TetrominoeManagerUniquePtr->Flip(TileMapUniquePtr->GetTiles(), TileMapUniquePtr->GetRows(), TileMapUniquePtr->GetCols());
}

void LevelManager::Subscribe()
{
	Start();
	if (!LockDelayManagerUniquePtr || !TetrominoeManagerUniquePtr)
	{
		return;
	}

	// register function callback for generating tetrominoe when the active one lock
	LockDelayManagerUniquePtr->GenerateTetrominoeOnLockDelegate = [&](uint8_t Rows, uint8_t Cols)
	{
		return
			(TetrominoeManagerUniquePtr != nullptr)
			? TetrominoeManagerUniquePtr->GenerateRandomTetromioe(Rows, Cols)
			: nullptr;
	};
}

void LevelManager::UnSubscribe()
{
	Stop();
	if (!LockDelayManagerUniquePtr)
	{
		return;
	}

	LockDelayManagerUniquePtr->GenerateTetrominoeOnLockDelegate = nullptr;
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
		LevelManagerPtr->PeriodicUpdate();
		return Interval;
	};

	PeriodicFunctorID = SDLTimerManager::CreateTimer(
		PeriodUpdateFunctor,
		Interval,
		const_cast<LevelManager*>(this)
	);

	// register new tetrominoe generation
	uint32_t(*GenerateTetrominoeOnLockFunctor)(uint32_t, void*) = [](uint32_t Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDLlogHelper::Print(PrefixErrorType::InvalidPtrInTimerDelegate, "LevelManager");
			exit(EXIT_FAILURE);
		}
		LevelManagerPtr->GenerateTetrominoeOnLock();
		return static_cast<uint32_t>(NULL);
	};

	OnLockFunctorID = SDLTimerManager::CreateTimer(
		GenerateTetrominoeOnLockFunctor,
		Interval,
		const_cast<LevelManager*>(this)
	);
}

void LevelManager::Stop()
{
	SDLTimerManager::DestroyTimer(PeriodicFunctorID);
	SDLTimerManager::DestroyTimer(OnLockFunctorID);
}

void LevelManager::PeriodicUpdate()
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

void LevelManager::GenerateTetrominoeOnLock()
{
	if (!LockDelayManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	LockDelayManagerUniquePtr->GenerateTetrominoeOnLockDelegate(
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
}