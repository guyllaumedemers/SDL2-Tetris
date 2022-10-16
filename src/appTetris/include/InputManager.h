#ifndef INCLUDED_INPUT_MANAGER
#define INCLUDED_INPUT_MANAGER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

class InputManager final
{
	typedef std::function<void(bool)> DelQuitGame;
	DelQuitGame QuitGameEvent;

	typedef std::function<void(int8_t, int8_t)> DelDirectionalKeyPressed;
	DelDirectionalKeyPressed DirectionalKeyPressedEvent;

	friend class GameManager;

public:
	InputManager() = default;
	~InputManager() = default;
	int WaitPollEvent(SDL_Event& Event) const;
};
#endif