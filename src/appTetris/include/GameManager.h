#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#endif

// --- forward declaration
class SDLManager;
class GameInstance;
class InputManager;
class TextureManager;
// ---

class GameManager final
{
	std::shared_ptr<GameInstance> GameInstanceSharedPtr = nullptr;
	std::shared_ptr<InputManager> InputManagerSharedPtr = nullptr;
	std::shared_ptr<TextureManager> TextureManagerSharedPtr = nullptr;
	std::shared_ptr<SDLManager> SDLManagerSharedPtr = nullptr;
	bool bIsQuittingGame = false;
public:
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager();
	~GameManager();
	GameManager& operator=(const GameManager&) = delete;
	GameManager& operator=(GameManager&&) = delete;
	int Run();
private:
	// Utils
	void Initialize();
	void Update();
	void Quit() const;
	void Subscribe();
	void UnSubscribe() const;
};
#endif
