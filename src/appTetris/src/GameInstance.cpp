#include "../include/GameInstance.h"

void GameInstance::Run() const
{
	if (!TileMapUniquePtr)
	{
		return;
	}

	/*Should retrieved values from Preset Difficulty*/
	const uint8_t&& tempRows = 20;
	const uint8_t&& tempCols = 12;
	TileMapUniquePtr->Init(tempRows, tempCols, [&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); });
}

void GameInstance::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!TileMapUniquePtr || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	/*Should handle Game Loop Refresh Rate so Tile Movement dont fly offscreen*/
	static constexpr int8_t&& OneDown = -1;
	static constexpr int8_t&& Idle = 0;
	TileMapUniquePtr->Update(TextureManagerPtrArg, TetrominoeManagerUniquePtr.get(), SDLManagerPtrArg, Idle, OneDown);
}

void GameInstance::Quit() const
{
	if (!TileMapUniquePtr)
	{
		return;
	}

	TileMapUniquePtr->Clear();
}

void GameInstance::PollKeyEvent(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg, int8_t DirX, int8_t DirY) const
{
	if (!TileMapUniquePtr || !TextureManagerPtrArg || !SDLManagerPtrArg)
	{
		return;
	}

	TileMapUniquePtr->Update(TextureManagerPtrArg, TetrominoeManagerUniquePtr.get(), SDLManagerPtrArg, DirX, DirY);
}