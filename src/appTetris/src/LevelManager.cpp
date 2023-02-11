#include "../include/LevelManager.h"
#include "../include/SDLManager.h"
#include "../include/TetrominoeManager.h"
#include "../include/TileMap.h"

#ifndef LOCK_DELAY
#define LOCK_DELAY 30.0
#endif

#ifndef PERIODIC_UPDATE
#define PERIODIC_UPDATE 60.0
#endif

void LevelManager::Initialize(SDLManager* const SDLManagerPtrArg, std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	if (!SDLManagerPtrArg || !TileMapUniquePtr || !TetrominoeManagerUniquePtr)
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
		TileMapUniquePtr->GetCols(),
		[&](uint16_t TetrominoeEntryIndex)
		{
			return (TileMapUniquePtr != nullptr) ? TileMapUniquePtr->CheckRowCompletion(TetrominoeEntryIndex) : NULL;
		}
	);

	// start tracking tetrominoe lock delay event
	InitializeLockDelaySystem(SDLManagerPtrArg);

	// reset level state
	ResetLockDelaySystem(SDLManagerPtrArg);
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
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOEMANAGER_PTR || TILEMAP_PTR ARE INVALID IN SPACE_PRESS_EVENT!");
		return;
	}

	TetrominoeManagerUniquePtr->Flip(TileMapUniquePtr->GetTiles(), TileMapUniquePtr->GetRows(), TileMapUniquePtr->GetCols());
}

uint32_t LevelManager::CreateNewTetrominoe(uint32_t Interval, void* Param) const
{
	LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
	if (!LevelManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: LEVEL_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR PERIODIC_UPDATE!");
		exit(EXIT_FAILURE);
	}

	TetrominoeManager* const TetrominoeManagerPtr = LevelManagerPtr->GetTetrominoeManager().get();
	if (!TetrominoeManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN LEVEL_MANAGER_INSTANCE CREATE_TETROMINOE!");
		exit(EXIT_FAILURE);
	}

	TetrominoeManagerPtr->GenerateRandomTetrominoeEvent();
	return static_cast<uint32_t>(NULL);
}

uint32_t LevelManager::PeriodicUpdate(uint32_t Interval, void* Param) const
{
	LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
	if (!LevelManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: LEVEL_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR PERIODIC_UPDATE!");
		exit(EXIT_FAILURE);
	}

	TetrominoeManager* const TetrominoeManagerPtr = LevelManagerPtr->GetTetrominoeManager().get();
	if (!TetrominoeManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN LEVEL_MANAGER_INSTANCE CREATE_TETROMINOE!");
		exit(EXIT_FAILURE);
	}

	TileMap* const TilemapManagerPtr = LevelManagerPtr->GetTilemapManager().get();
	if (!TilemapManagerPtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TILEMAP_MANAGER_PTR INVALID IN LEVEL_MANAGER_INSTANCE CREATE_TETROMINOE!");
		exit(EXIT_FAILURE);
	}

	static constexpr int8_t&& OneDown = -1;
	static constexpr int8_t&& Idle = 0;

	// update tetrominoes
	TetrominoeManagerPtr->Update(
		TilemapManagerPtr->GetTiles(),
		Idle,
		OneDown,
		TilemapManagerPtr->GetRows(),
		TilemapManagerPtr->GetCols()
	);

	return Interval;
}

void LevelManager::InitializeLockDelaySystem(SDLManager* const SDLManagerPtrArg)
{
	if (!SDLManagerPtrArg || !TetrominoeManagerUniquePtr)
	{
		return;
	}

	// delegate callback to reset locking system that allow tetrominoes to move
	LockDelayCompletedEvent = [&]()
	{
		GetTetrominoeManager().get()->LockActiveTetrominoeEvent();
		ResetLockDelaySystem(SDLManagerPtrArg);
	};

	// delegate callback to track and update the state of the active tetrominoe and set its lock state when timer complete
	TetrominoeManagerUniquePtr->ResetTetrominoeLockDelayTimerEvent = [&](int ActiveLockDelay)
	{
		static constexpr double&& Fps = (1.f / LOCK_DELAY);
		static constexpr uint16_t&& Miliseconds = 1000;
		static const Uint32&& DelayInMiliseconds = static_cast<Uint32>(std::floor(Fps * Miliseconds));

		if (ActiveLockDelay != NULL)
		{
			bool IsDestroyed = DestroyTimer(SDLManagerPtrArg, ActiveLockDelay);
			if (!IsDestroyed)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TIMER LOCK DELAY DESTRUCTION FAILED!");
				//exit(EXIT_FAILURE);
			}
		}

		uint32_t(*ResetLockDelayFunctor)(uint32_t, void*) = [](uint32_t Interval, void* Param)
		{
			LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
			if (!LevelManagerPtr)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: LEVEL_MANAGER_PTR ID INVALID IN RESET_LOCK_DELAY_EVENT!");
				exit(EXIT_FAILURE);
			}

			LevelManagerPtr->LockDelayCompletedEvent();
			return static_cast<uint32_t>(NULL);
		};

		return CreateTimer(SDLManagerPtrArg, ResetLockDelayFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
	};
}

void LevelManager::ResetLockDelaySystem(SDLManager* const SDLManagerPtrArg)
{
	static constexpr double&& Fps = (1.f / PERIODIC_UPDATE);
	static constexpr uint16_t&& Miliseconds = 1000;
	static const Uint32&& DelayInMiliseconds = static_cast<Uint32>(std::floor(Fps * Miliseconds));

	// --- static TimerID
	static int TimerID_A = (TimerID_A != NULL) ? TimerID_A : NULL;
	static int TimerID_B = (TimerID_B != NULL) ? TimerID_B : NULL;
	// ---

	// reset timers
	DestroyTimer(SDLManagerPtrArg, TimerID_A);
	DestroyTimer(SDLManagerPtrArg, TimerID_B);

	// create functors
	Uint32(*CreateNewTetrominoeFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR CREATE_TETROMINOE!");
			exit(EXIT_FAILURE);
		}

		return LevelManagerPtr->CreateNewTetrominoe(Interval, Param);
	};

	Uint32(*PeriodicUpdateFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR PERIODIC_UPDATE!");
			exit(EXIT_FAILURE);
		}

		return LevelManagerPtr->PeriodicUpdate(Interval, Param);
	};

	// start timers
	TimerID_A = CreateTimer(SDLManagerPtrArg, CreateNewTetrominoeFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
	TimerID_B = CreateTimer(SDLManagerPtrArg, PeriodicUpdateFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
}

int LevelManager::CreateTimer(SDLManager* const SDLManagerPtrArg, uint32_t(*Functor)(uint32_t, void*), uint32_t TimerDelay, void* VoidPtrArg)
{
	if (!SDLManagerPtrArg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN GAME_MANAGER_INSTANCE START_TIMER!");
		exit(EXIT_FAILURE);
	}

	return SDLManagerPtrArg->GetTimer()
		.Start(
			TimerDelay,
			Functor,
			VoidPtrArg
		);
}

int LevelManager::DestroyTimer(SDLManager* const SDLManagerPtrArg, int TimerID)
{
	if (!SDLManagerPtrArg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN GAME_MANAGER_INSTANCE START_TIMER!");
		exit(EXIT_FAILURE);
	}

	return SDLManagerPtrArg->GetTimer()
		.Stop(TimerID);
}