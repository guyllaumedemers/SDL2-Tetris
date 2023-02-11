#ifndef INCLUDED_GAME_MANAGER
#define INCLUDED_GAME_MANAGER

#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include <memory>
#endif

#ifndef FRAME_RATE
#define FRAME_RATE 60.0
#endif

#include "SDLManager.h"
#include "GameInstance.h"
#include "InputManager.h"
#include "TextureManager.h"

class GameManager final
{
	std::unique_ptr<SDLManager> SDLManagerUniquePtr = std::make_unique<SDLManager>();
	std::unique_ptr<GameInstance> GameInstanceUniquePtr = std::make_unique<GameInstance>();
	std::unique_ptr<InputManager> InputManagerUniquePtr = std::make_unique<InputManager>();
	std::unique_ptr<TextureManager> TextureManagerUniquePtr = std::make_unique<TextureManager>();
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
	void Initialize();
	void Update();
	void Quit() const;
	void Subscribe();
	void UnSubscribe() const;
};
#endif
