#include "../include/GameInstance.h"

#include <string> //Temp, will remove

void GameInstance::Play()
{
}

void GameInstance::Update()
{
}

void GameInstance::Pause()
{
}

void GameInstance::Quit()
{
}

void GameInstance::PollKeyEvent(int DirX, int DirY)
{
	std::sprintf("{0} {1}", std::to_string(DirX).c_str(), std::to_string(DirY).c_str()); /*Invoke game logic for Tetrominos movement*/
}