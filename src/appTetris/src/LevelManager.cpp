#include "../include/LevelManager.h"
#include "../include/SDLManager.h"
#include "../include/TetrominoeManager.h"
#include "../include/TileMap.h"

#ifndef LOCK_DELAY
#define LOCK_DELAY 30.0
#endif

#ifndef PERIODIC_UPDATE
#define PERIODIC_UPDATE 10.0
#endif

void LevelManager::Initialize(SDLManager* const SDLManagerPtrArg, std::function<void(uint16_t, uint16_t)> SetWindowEvent)
{
	if (!SDLManagerPtrArg)
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

	Reset(SDLManagerPtrArg);
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

void LevelManager::Reset(SDLManager* const SDLManagerPtrArg)
{
	Stop();
	Start(SDLManagerPtrArg);
}

void LevelManager::Start(SDLManager* const SDLManagerPtrArg)
{
	static constexpr double&& Fps = (1.f / PERIODIC_UPDATE);
	static constexpr uint16_t&& Miliseconds = 1000;
	static const Uint32&& DelayInMiliseconds = static_cast<Uint32>(std::floor(Fps * Miliseconds));

	// create functors
	Uint32(*CreateNewTetrominoeFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR CREATE_TETROMINOE!");
			return Interval;
		}

		return LevelManagerPtr->CreateNewTetrominoe(Interval, Param);
	};

	Uint32(*PeriodicUpdateFunctor)(Uint32, void*) = [](Uint32 Interval, void* Param)
	{
		LevelManager* const LevelManagerPtr = static_cast<LevelManager*>(Param);
		if (!LevelManagerPtr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN SDL_TIMER FUNCTOR PERIODIC_UPDATE!");
			return Interval;
		}

		return LevelManagerPtr->PeriodicUpdate(Interval, Param);
	};

	// start timers
	MakeTimer(SDLManagerPtrArg, CreateNewTetrominoeFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
	MakeTimer(SDLManagerPtrArg, PeriodicUpdateFunctor, DelayInMiliseconds, const_cast<LevelManager*>(this));
}

void LevelManager::Stop()
{
}

uint32_t LevelManager::CreateNewTetrominoe(uint32_t Interval, void* Params) const
{
	if (!TetrominoeManagerUniquePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN LEVEL_MANAGER_INSTANCE CREATE_TETROMINOE!");
		return Interval;
	}

	TetrominoeManagerUniquePtr->GenerateRandomTetrominoeEvent();
	return static_cast<uint32_t>(NULL);
}

uint32_t LevelManager::PeriodicUpdate(uint32_t Interval, void* Params) const
{
	if (!TetrominoeManagerUniquePtr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: TETROMINOE_MANAGER_PTR INVALID IN LEVEL_MANAGER_INSTANCE PERIODIC_UPDATE!");
		return Interval;
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

	return Interval;
}

void LevelManager::MakeTimer(SDLManager* const SDLManagerPtrArg, uint32_t(*Functor)(uint32_t, void*), uint32_t TimerDelay, void* VoidPtrArg)
{
	if (!SDLManagerPtrArg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: SDL_MANAGER_PTR INVALID IN GAME_MANAGER_INSTANCE START_TIMER!");
		exit(EXIT_FAILURE);
	}

	SDLManagerPtrArg->GetTimer().Start(
		TimerDelay,
		Functor,
		VoidPtrArg
	);
}
