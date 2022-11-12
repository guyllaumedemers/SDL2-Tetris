#include "../include/GameInstance.h"

void GameInstance::Play() const
{
	if (!TileMapPtr)
	{
		return;
	}

	/*Should retrieved values from Preset Difficulty*/
	const uint8_t&& tempRows = 20;
	const uint8_t&& tempCols = 12;
	TileMapPtr->Init(tempRows, tempCols, [&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); });
}

void GameInstance::Update(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr) const
{
	if (!TileMapPtr || !TextureManagerPtr || !SDLManagerPtr)
	{
		return;
	}

	/*Should handle Game Loop Refresh Rate so Tile Movement dont fly offscreen*/
	static constexpr int8_t&& OneDown = -1;
	static constexpr int8_t&& Idle = 0;
	TileMapPtr->Update(TextureManagerPtr, TetrominoeManagerPtr.get(), SDLManagerPtr, Idle, OneDown);
}

void GameInstance::Pause() const
{
}

void GameInstance::Quit() const
{
	if (!TileMapPtr)
	{
		return;
	}

	TileMapPtr->Clear();
}

void GameInstance::PollKeyEvent(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, int8_t DirX, int8_t DirY) const
{
	if (!TileMapPtr || !TextureManagerPtr || !SDLManagerPtr)
	{
		return;
	}

	TileMapPtr->Update(TextureManagerPtr, TetrominoeManagerPtr.get(), SDLManagerPtr, DirX, DirY);
}