#ifndef INCLUDED_LEVELMANAGER
#define INCLUDED_LEVELMANAGER

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

class LevelManager final
{
	std::unique_ptr<TileMap> TileMapUniquePtr = std::make_unique<TileMap>();
	std::unique_ptr<TetrominoeManager> TetrominoeManagerUniquePtr = std::make_unique<TetrominoeManager>();
public:
	LevelManager(const LevelManager&) = delete;
	LevelManager(LevelManager&&) = delete;
	LevelManager() = default;
	~LevelManager() = default;
	LevelManager& operator=(const LevelManager&) = delete;
	LevelManager& operator=(LevelManager&&) = delete;
	void Initialize(class SDLManager* const SDLManagerPtrArg, std::function<void(uint16_t, uint16_t)> SetWindowEvent);
	void Update(class TextureManager* const TextureManagerPtrArg, class SDLManager* const SDLManagerPtrArg) const;
	void Clear() const;
	void PollKeyEvent(int8_t DirX, int8_t DirY) const;
	void PollSpaceKeyEvent() const;
private:
	// --- Getter/Setter
	const std::unique_ptr<TileMap>& GetTilemapManager() const { return TileMapUniquePtr; }
	const std::unique_ptr<TetrominoeManager>& GetTetrominoeManager() const { return TetrominoeManagerUniquePtr; }
	// --- Timers
	int CreateTimer(class SDLManager* const SDLManagerPtrArg, uint32_t(*Functor)(uint32_t, void*), uint32_t TimerDelay, void* VoidPtrArg);
	int DestroyTimer(class SDLManager* const SDLManagerPtrArg, int TimerID);
	// --- Lock Delay system
	void InitializeLockDelaySystem(class SDLManager* const SDLManagerPtrArg);
	void ResetLockDelaySystem(class SDLManager* const SDLManagerPtrArg);
	// --- Functor
	uint32_t CreateNewTetrominoe(uint32_t Interval, void* Param) const;
	uint32_t PeriodicUpdate(uint32_t Interval, void* Param) const;
	// --- Delegates
	typedef std::function<void()> DelLockDelayCompleted;
	DelLockDelayCompleted LockDelayCompletedEvent;
	// ---
};
#endif
