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
#include "TetrominoeManager.h"

class GameInstance final
{
	typedef std::function<void(uint16_t, uint16_t)> DelSetWindow;
	DelSetWindow SetWindowEvent;

	std::unique_ptr<TileMap> TileMapPtr = std::make_unique<TileMap>();
	std::unique_ptr<TetrominoeManager> TetrominoeManagerPtr = std::make_unique<TetrominoeManager>();
	friend class GameManager;
public:
	GameInstance(const GameInstance&) = delete;
	GameInstance(GameInstance&&) = delete;
	GameInstance() = default;
	~GameInstance() = default;
	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance& operator=(GameInstance&&) = delete;
	void Play() const;
	void Update(class TextureManager* const TextureManagerPtr, class SDLManager* const SDLManagerPtr) const;
	void Pause() const;
	void Quit() const;
	void PollKeyEvent(class TextureManager* const TextureManagerPtr, class SDLManager* const SDLManagerPtr, int8_t DirX, int8_t DirY) const;
};
#endif
