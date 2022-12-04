#include "../include/GameInstance.h"
#include "../include/SDLManager.h"

void GameInstance::Initialize(SDLManager* const SDLManagerPtrArg) const
{
	if (!TileMapUniquePtr || !TetrominoeManagerUniquePtr || !SDLManagerPtrArg)
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
		TileMapUniquePtr.get()
	);

	const uint8_t&& CallbackDelay = static_cast<uint8_t>(3 * 1000);

	// create functor
	Uint32(*Functor)(Uint32, void*) = [](Uint32 Interval, void* Params)
	{
		static_cast<TetrominoeManager*>(Params)->GenerateRandomTetrominoeEvent();
		// do not want to repeat this first invoke, so we run 0
		return static_cast<Uint32>(NULL);
	};

	// initialize sdl timer callback
	SDLManagerPtrArg->GetTimer().Start(CallbackDelay, Functor, TetrominoeManagerUniquePtr.get());
}

void GameInstance::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!TileMapUniquePtr || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	static constexpr int8_t&& OneDown = -1;
	static constexpr int8_t&& Idle = 0;

	// update tetrominoes
	TetrominoeManagerUniquePtr->Update(
		TileMapUniquePtr.get(),
		Idle,
		OneDown,
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);

	// update tilemap
	TileMapUniquePtr->Update(
		TextureManagerPtrArg,
		SDLManagerPtrArg
	);
}

void GameInstance::Clear() const
{
	if (!TileMapUniquePtr || !TetrominoeManagerUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Clear();
	TileMapUniquePtr->Clear();
}

void GameInstance::PollKeyEvent(int8_t DirX, int8_t DirY) const
{
	if (!TetrominoeManagerUniquePtr || !TileMapUniquePtr)
	{
		return;
	}

	TetrominoeManagerUniquePtr->Update(
		TileMapUniquePtr.get(),
		DirX,
		DirY,
		TileMapUniquePtr->GetRows(),
		TileMapUniquePtr->GetCols()
	);
}