#ifndef INCLUDED_GAME_INSTANCE
#define INCLUDED_GAME_INSTANCE

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#include "TileMap.h"

class GameInstance final
{
	typedef std::function<void(uint16_t, uint16_t)> DelSetWindow;
	DelSetWindow SetWindowEvent;

	std::unique_ptr<TileMap> GridTileMap = std::make_unique<TileMap>();

	friend class GameManager;

public:
	GameInstance() = default;
	~GameInstance() = default;
	void Play();
	void Update();
	void Pause();
	void Quit();
	void PollKeyEvent(int8_t DirX, int8_t DirY);
};
#endif
