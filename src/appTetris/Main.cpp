#include "include/GameManager.h"

int main(int argc, char** argv)
{
    std::unique_ptr<GameManager> const GameManagerPtr = std::make_unique<GameManager>();
    if(!GameManagerPtr)
    {
        return EXIT_FAILURE;
    }
    return GameManagerPtr->Run();
}
