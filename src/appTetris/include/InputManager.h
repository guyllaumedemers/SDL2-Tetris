#ifndef INCLUDED_INPUT_MANAGER
#define INCLUDED_INPUT_MANAGER

#ifndef INCLUDED_SDL_LIB
#define INCLUDED_SDL_LIB
#include <SDL.h>
#endif

#ifndef INCLUDED_CSTD_INT
#define INCLUDED_CSTD_INT
#include <cstdint>
#endif

#ifndef INCLUDED_FUNCTIONAL
#define INCLUDED_FUNCTIONAL
#include <functional>
#endif

class InputManager final
{
public:
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager() = default;
	~InputManager() = default;
	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(InputManager&&) = delete;
	int WaitPollEvent(SDL_Event& Event) const;
	// --- Delegate
	typedef std::function<void(int8_t, int8_t)> DelDirectionalKeyPressed;
	DelDirectionalKeyPressed DirectionalKeyPressedEvent;

	typedef std::function<void()> DelSpaceKeyPressed;
	DelSpaceKeyPressed DelSpaceKeyPressedEvent;

	typedef std::function<void(bool)> DelQuitGame;
	DelQuitGame QuitGameEvent;
	// --
};
#endif