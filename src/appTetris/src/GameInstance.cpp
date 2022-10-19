#include "../include/GameInstance.h"

void GameInstance::Play()
{
	if (!GridTileMap)
	{
		return;
	}
	/*Should retrieved values from Preset Difficulty*/
	const uint8_t Rows = 20;
	const uint8_t Cols = 12;
	GridTileMap->Init(Rows, Cols, [&](uint16_t Rows, uint16_t Cols) { SetWindowEvent(Rows, Cols); });
}

void GameInstance::Update()
{
	if (!GridTileMap)
	{
		return;
	}
	/*Should handle Game Loop Refresh Rate so Tile Movement dont fly offscreen*/
	static constexpr int8_t OneDown = -1;
	static constexpr int8_t Idle = 0;
	GridTileMap->Update(Idle, OneDown);
}

void GameInstance::Pause()
{
}

void GameInstance::Quit()
{
	if (!GridTileMap)
	{
		return;
	}
	GridTileMap->Clear();
}

void GameInstance::PollKeyEvent(int8_t DirX, int8_t DirY)
{
	if (!GridTileMap)
	{
		return;
	}
	GridTileMap->Update(DirX, DirY);
}