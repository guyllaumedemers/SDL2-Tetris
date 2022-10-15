#include <SDL.h>
#include <cstdlib>

void libImGUITest();
//void libSDL2Test();

int main(int argc, char** argv)
{
    libImGUITest();
    //libSDL2Test();

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 INIT FAILED!");
        return EXIT_FAILURE;
    }

    SDL_Window* Window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 200, NULL);

    if(!Window)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 WINDOW CREATION FAILED!");
        return EXIT_FAILURE;
    }

    bool bIsWindowOpen = true;

    while(bIsWindowOpen)
    {
        SDL_Event Event;
	    if(SDL_WaitEvent(&Event) > 0)
	    {
		    if(Event.type == SDL_QUIT)
		    {
                bIsWindowOpen = false;
		    }
	    }
    }

    SDL_DestroyWindow(Window);
    SDL_Quit();

    return 0;
}
