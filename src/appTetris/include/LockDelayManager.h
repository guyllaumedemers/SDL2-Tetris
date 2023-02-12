#ifndef LOCKDELAYMANAGER_H
#define LOCKDELAYMANAGER_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

// --- forward declaration
class TetrominoeManager;
class Tetrominoe;
// ---

class LockDelayManager
{
public:
	LockDelayManager(const LockDelayManager&) = delete;
	LockDelayManager(LockDelayManager&&) = delete;
	LockDelayManager() = default;
	~LockDelayManager();
	LockDelayManager& operator==(const LockDelayManager&) = delete;
	LockDelayManager& operator==(LockDelayManager&&) = delete;
public:
	// Delegates
	typedef std::function<void(uint8_t, uint8_t)> GenerateTetrominoeOnLockEvent;
	GenerateTetrominoeOnLockEvent GenerateTetrominoeOnLockDelegate;
};
#endif
