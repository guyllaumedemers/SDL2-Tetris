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
	if (!TileMapUniquePtr || !TetrominoeManagerUniquePtr)
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

	// initialize events in tetrominoe manager
	TetrominoeManagerUniquePtr->Initialize(
		TileMapUniquePtr->GetTiles(),
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
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

void LevelManager::Clear() const
{
	if (!TileMapUniquePtr || !TetrominoeManagerUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Clear();
	TileMapUniquePtr->Clear();
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

//void LevelManager::InitializeLockDelaySystem(SDLManager* const SDLManagerPtrArg)
//{
//	if (!SDLManagerPtrArg || !TetrominoeManagerUniquePtr)
//	{
//		return;
//	}
//
//	// delegate callback to reset locking system that allow tetrominoes to move
//	LockDelayCompletedEvent = [&]()
//	{
//		GetTetrominoeManager().get()->LockActiveTetrominoeEvent();
//		ResetLockDelaySystem(SDLManagerPtrArg);
//	};
//
//	// delegate callback to track and update the state of the active tetrominoe and set its lock state when timer complete
//	TetrominoeManagerUniquePtr->ResetTetrominoeLockDelayTimerEvent = [&](int ActiveLockDelay)
//	{
//		static constexpr double&& Fps = (1.f / LOCK_DELAY);
//		static constexpr uint16_t&& Miliseconds = 1000;
//		static const Uint32&& DelayInMiliseconds = static_cast<Uint32>(std::floor(Fps * Miliseconds));
//
//		if (ActiveLockDelay != NULL)
//		{
//			bool IsDestroyed = DestroyTimer(SDLManagerPtrArg, ActiveLockDelay);
//			if (!IsDestroyed)
//			{
//				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TIMER LOCK DELAY DESTRUCTION FAILED!");
//				//exit(EXIT_FAILURE);
//			}
//		}
//
//		uint32_t(*ResetLockDelayFunctor)(uint32_t, void*) = [](uint32_t Interval, void* Param)
//		{
//			LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
//			if (!LevelManagerPtr)
//			{
//				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: LEVEL_MANAGER_PTR ID INVALID IN RESET_LOCK_DELAY_EVENT!");
//				exit(EXIT_FAILURE);
//			}
//
//			LevelManagerPtr->LockDelayCompletedEvent();
//			return static_cast<uint32_t>(NULL);
//		};
//
//		return CreateTimer(SDLManagerPtrArg, ResetLockDelayFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
//	};
//}
//
//void LevelManager::ResetLockDelaySystem(SDLManager* const SDLManagerPtrArg)
//{
//	static constexpr double&& Fps = (1.f / PERIODIC_UPDATE);
//	static constexpr uint16_t&& Miliseconds = 1000;
//	static const Uint32&& DelayInMiliseconds = static_cast<Uint32>(std::floor(Fps * Miliseconds));
//
//	// --- static TimerID
//	static int TimerID_A = (TimerID_A != NULL) ? TimerID_A : NULL;
//	static int TimerID_B = (TimerID_B != NULL) ? TimerID_B : NULL;
//	// ---
//
//	// reset timers
//	DestroyTimer(SDLManagerPtrArg, TimerID_A);
//	DestroyTimer(SDLManagerPtrArg, TimerID_B);
//
//	// create functors
//	Uint32(*CreateNewTetrominoeFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
//	{
//		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
//		if (!LevelManagerPtr)
//		{
//			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR CREATE_TETROMINOE!");
//			exit(EXIT_FAILURE);
//		}
//
//		return LevelManagerPtr->CreateNewTetrominoe(Interval, Param);
//	};
//
//	Uint32(*PeriodicUpdateFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
//	{
//		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
//		if (!LevelManagerPtr)
//		{
//			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR PERIODIC_UPDATE!");
//			exit(EXIT_FAILURE);
//		}
//
//		return LevelManagerPtr->PeriodicUpdate(Interval, Param);
//	};
//
//	// start timers
//	TimerID_A = CreateTimer(SDLManagerPtrArg, CreateNewTetrominoeFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
//	TimerID_B = CreateTimer(SDLManagerPtrArg, PeriodicUpdateFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
//}