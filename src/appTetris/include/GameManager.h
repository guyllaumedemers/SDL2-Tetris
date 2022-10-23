#ifndef INCLUDED_GAME_MANAGER
#define INCLUDED_GAME_MANAGER

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#include "SDLManager.h"
#include "InputManager.h"
#include "GameInstance.h"

class GameManager final
{
	std::unique_ptr<SDLManager> SDLManagerPtr = std::make_unique<SDLManager>();
	std::unique_ptr<InputManager> InputManagerPtr = std::make_unique<InputManager>();
	std::unique_ptr<GameInstance> GameInstancePtr = std::make_unique<GameInstance>();
	bool bIsQuittingGame = false;
public:
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager() = default;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	GameManager& operator=(GameManager&&) = delete;
	int Run();
private:
	void Init();
	void Update();
	void Clear();
	void Subscribe();
	void UnSubscribe();
};
#endif
