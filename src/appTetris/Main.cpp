#include "include/GameManager.h"
// https://stackoverflow.com/questions/13865842/does-static-constexpr-variable-inside-a-function-make-sense

int main(int argc, char** argv)
{
	std::unique_ptr<GameManager>&& GameManagerPtr = std::make_unique<GameManager>();
	return (GameManagerPtr != nullptr) ? GameManagerPtr->Run() : EXIT_FAILURE;
}
