#ifndef INCLUDED_GAME_INSTANCE
#define INCLUDED_GAME_INSTANCE

class GameInstance final
{

public:
	GameInstance() = default;
	~GameInstance() = default;
	void Play();
	void Update();
	void Pause();
	void Quit();
	void PollKeyEvent(int DirX, int DirY);
};
#endif
