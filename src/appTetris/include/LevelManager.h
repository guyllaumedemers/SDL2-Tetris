#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

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
class TileMap;
class LockDelayManager;
class TetrominoeManager;
class TextureManager;
class SDLManager;
// ---

class LevelManager final
{
	std::shared_ptr<TetrominoeManager> TetrominoeManagerUniquePtr = nullptr;
	std::shared_ptr<TileMap> TileMapUniquePtr = nullptr;
	std::shared_ptr<LockDelayManager> LockDelayManagerUniquePtr = nullptr;
	static inline int DelayedStartFunctorID = NULL;
	static inline int PeriodicFunctorID = NULL;
	static inline int OnLockFunctorID = NULL;
public:
	LevelManager(const LevelManager&) = delete;
	LevelManager(LevelManager&&) = delete;
	LevelManager();
	~LevelManager();
	LevelManager& operator=(const LevelManager&) = delete;
	LevelManager& operator=(LevelManager&&) = delete;
	void Initialize(std::function<void(uint16_t, uint16_t)> SetWindowEvent);
	void Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const;
	void Clear();
	void Reset(std::function<void(uint16_t, uint16_t)> SetWindowEvent);
	void PollKeyEvent(int8_t DirX, int8_t DirY) const;
	void PollSpaceKeyEvent() const;
private:
	void Subscribe();
	void UnSubscribe();
	// Utils
	void Start();
	void Stop();
public:
	// timer callback
	void PeriodicUpdate();
	void GenerateTetrominoeOnLock();
};
#endif
