#ifndef INCLUDED_GAME_INSTANCE
#define INCLUDED_GAME_INSTANCE

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "TileMap.h"

class GameInstance final
{
	std::unique_ptr<TileMap> GridTileMap = std::make_unique<TileMap>();

public:
	GameInstance() = default;
	~GameInstance() = default;
	void Play();
	void Update();
	void Pause();
	void Quit();
	void PollKeyEvent(int DirX, int DirY);
};
#endif
