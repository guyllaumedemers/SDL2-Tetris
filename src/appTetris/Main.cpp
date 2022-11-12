#include "include/GameManager.h"

int main(int argc, char** argv)
{
	std::unique_ptr<GameManager> const GameManagerPtr = std::make_unique<GameManager>();
	return (GameManagerPtr != nullptr) ? GameManagerPtr->Run() : EXIT_FAILURE;
}
