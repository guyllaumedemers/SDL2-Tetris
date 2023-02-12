#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

#ifndef CSTDINT_H
#define CSTDINT_H
#include <cstdint>
#endif

// --- forward declaration
class LevelManager;
class TextureManager;
class SDLManager;
// ---

class GameInstance final
{
	std::shared_ptr<LevelManager> LevelManagerUniquePtr = nullptr;
public:
	GameInstance(const GameInstance&) = delete;
	GameInstance(GameInstance&&) = delete;
	GameInstance();
	~GameInstance();
	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance& operator=(GameInstance&&) = delete;
	void Initialize() const;
	void Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const;
	void Clear() const;
	void Reset();
	void PollKeyEvent(int8_t DirX, int8_t DirY) const;
	void PollSpaceKeyEvent() const;
public:
	// Delegates
	typedef std::function<void(uint16_t, uint16_t)> SetWindowEvent;
	SetWindowEvent SetWindowDelegate;
};
#endif
