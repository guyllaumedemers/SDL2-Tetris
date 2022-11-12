#include "../include/GameInstance.h"

void GameInstance::Play()
{
	if (!TileMapPtr)
	{
		return;
	}

	/*Should retrieved values from Preset Difficulty*/
	const uint8_t&& Rows = 20;
	const uint8_t&& Cols = 12;
	TileMapPtr->Init(Rows, Cols, [&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); });
}

void GameInstance::Update(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr)
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

void GameInstance::Pause()
{
}

void GameInstance::Quit()
{
	if (!TileMapPtr)
	{
		return;
	}

	TileMapPtr->Clear();
}

void GameInstance::PollKeyEvent(TextureManager* const TextureManagerPtr, SDLManager* const SDLManagerPtr, int8_t DirX, int8_t DirY)
{
	if (!TileMapPtr || !TextureManagerPtr || !SDLManagerPtr)
	{
		return;
	}

	TileMapPtr->Update(TextureManagerPtr, TetrominoeManagerPtr.get(), SDLManagerPtr, DirX, DirY);
}