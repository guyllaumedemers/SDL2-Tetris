#include "include/GameManager.h"
// https://tetris.wiki/Tetris_Guideline
// https://tetris.wiki/Sega_Rotation
// https://tetris.wiki/Arika_Rotation_System
// https://tetris.wiki/Super_Rotation_System
// https://tetris.wiki/Rotate#IRS
// https://tetris.wiki/DAS
// https://tetris.wiki/Lock_delay
// https://tetris.wiki/Drop

int main(int argc, char** argv)
{
	std::unique_ptr<GameManager>&& GameManagerPtr = std::make_unique<GameManager>();
	return (GameManagerPtr != nullptr) ? GameManagerPtr->Run() : EXIT_FAILURE;
}
