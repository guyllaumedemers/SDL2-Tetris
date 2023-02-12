#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

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

class SDLTimerManager final
{
	typedef bool SDLResult;
	typedef int SDLTimerID;

	// aggregated timer handle definition
	struct SDLTimer final
	{
		SDLTimerID Start(uint32_t CallbackDelay, uint32_t(*CallbackEvent)(uint32_t, void*), void* Param);
		SDLResult Stop(SDLTimerID TimerID);
	};

	static std::unique_ptr<SDLTimerManager> Singleton;
	SDLTimer TimerHandle;
public:
	SDLTimerManager(const SDLTimerManager&) = delete;
	SDLTimerManager(SDLTimerManager&&) = delete;
	SDLTimerManager() = default;
	~SDLTimerManager() = default;
	SDLTimerManager& operator==(const SDLTimerManager&) = delete;
	SDLTimerManager& operator==(SDLTimerManager&&) = delete;
	static int CreateTimer(uint32_t(*Functor)(uint32_t, void*), uint32_t TimerDelay, void* Param);
	static int DestroyTimer(int TimerID);
public:
	// Getter/Setter
	SDLTimer& GetTimerHandle() { return TimerHandle; }
private:
	// Getter/Setter
	static SDLTimerManager* Get();
};
#endif