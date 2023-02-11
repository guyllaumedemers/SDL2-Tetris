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

#include "LevelManager.h"

class GameInstance final
{
	std::unique_ptr<LevelManager> LevelManagerUniquePtr = std::make_unique<LevelManager>();
public:
	GameInstance(const GameInstance&) = delete;
	GameInstance(GameInstance&&) = delete;
	GameInstance() = default;
	~GameInstance() = default;
	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance& operator=(GameInstance&&) = delete;
	void Initialize(class SDLManager* const SDLManagerPtrArg) const;
	void Update(class TextureManager* const TextureManagerPtrArg, class SDLManager* const SDLManagerPtrArg) const;
	void Clear() const;
	void PollKeyEvent(int8_t DirX, int8_t DirY) const;
	void PollSpaceKeyEvent() const;
	// --- Delegate
	typedef std::function<void(uint16_t, uint16_t)> DelSetWindow;
	DelSetWindow SetWindowEvent;
	// ---
};
#endif
