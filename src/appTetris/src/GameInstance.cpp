#include "../include/GameInstance.h"

void GameInstance::Play()
{
	if (!TileMapPtr)
	{
		return;
	}
	/*Should retrieved values from Preset Difficulty*/
	const uint8_t Rows = 20;
	const uint8_t Cols = 12;
	TileMapPtr->Init(Rows, Cols, [&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); });
}

void GameInstance::Update()
{
	if (!TileMapPtr)
	{
		return;
	}
	/*Should handle Game Loop Refresh Rate so Tile Movement dont fly offscreen*/
	static constexpr int8_t OneDown = -1;
	static constexpr int8_t Idle = 0;
	TileMapPtr->Update(TetrominoeManagerPtr.get(), Idle, OneDown);
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

void GameInstance::PollKeyEvent(int8_t DirX, int8_t DirY)
{
	if (!TileMapPtr)
	{
		return;
	}
	TileMapPtr->Update(TetrominoeManagerPtr.get(), DirX, DirY);
}