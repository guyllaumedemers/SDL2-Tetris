#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <functional>
#endif

#ifndef CTSDINT_H
#define CTSDINT_H
#include <cstdint>
#endif

// --- forward declaration
union SDL_Event;
// ---

class InputManager final
{
public:
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager() = default;
	~InputManager();
	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(InputManager&&) = delete;
	void RunEvent(SDL_Event* Event) const;
private:
	// Delegates
	typedef std::function<void(int8_t, int8_t)> DirectionalKeyPressedEvent;
	DirectionalKeyPressedEvent DirectionalKeyPressedDelegate;

	typedef std::function<void()> SpaceKeyPressedEvent;
	SpaceKeyPressedEvent SpaceKeyPressedDelegtate;

	typedef std::function<void(bool)> QuitGameEvent;
	QuitGameEvent QuitGameDelegate;
public:
	// Getter/Setter
	DirectionalKeyPressedEvent& GetDirectionalKeyPressedDel() { return DirectionalKeyPressedDelegate; }
	SpaceKeyPressedEvent& GetSpaceKeyPressedDel() { return SpaceKeyPressedDelegtate; }
	QuitGameEvent& GetQuitGameDel() { return QuitGameDelegate; }
};
#endif