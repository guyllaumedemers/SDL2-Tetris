#include "../include/GameInstance.h"

void GameInstance::Play()
{
}

void GameInstance::Update()
{
	if(!GridTileMap)
	{
		static_assert(GridTileMap);
		return;
	}
	/*Should handle Game Loop Refresh Rate so Tile Movement */
	static constexpr uint8_t OneDown = -1;
	static constexpr uint8_t Idle = 0;
	GridTileMap->Update(Idle, OneDown);
}

void GameInstance::Pause()
{
}

void GameInstance::Quit()
{
}

void GameInstance::PollKeyEvent(int DirX, int DirY)
{
	if (!GridTileMap)
	{
		static_assert(GridTileMap);
		return;
	}
	GridTileMap->Update(DirX, DirY);
}