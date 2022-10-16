#ifndef INCLUDED_GAME_MANAGER
#define INCLUDED_GAME_MANAGER

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "SDLManager.h"
#include "InputManager.h"

class GameManager final
{
	std::unique_ptr<SDLManager> SDLManagerPtr = std::make_unique<SDLManager>();
	std::unique_ptr<InputManager> InputManagerPtr = std::make_unique<InputManager>();
	bool bIsQuittingGame = false;

public:
	GameManager() = default;
	~GameManager() = default;
	int Run();

private:
	int Init();
	void Update();
	void Clear();
};
#endif
