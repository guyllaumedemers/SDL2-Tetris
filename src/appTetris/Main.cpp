#include "include/SDLHandle.h"
#include <cstdlib>

void libImGUITest();
//void libSDL2Test();

int main(int argc, char** argv)
{
    libImGUITest();
    //libSDL2Test();

    std::unique_ptr<SDLHandle> const pSDLHandle = std::make_unique<SDLHandle>();
    if(!pSDLHandle)
    {
        return EXIT_FAILURE;
    }
    
    bool bIsWindowOpen = true;
    pSDLHandle->Init();
    while (bIsWindowOpen)
    {
        SDL_Event Event;
        if (SDL_WaitEvent(&Event) > 0)
        {
            if (Event.type == SDL_QUIT)
            {
                bIsWindowOpen = false;
            }
        }
    	pSDLHandle->Update();
    }
    pSDLHandle->Clear();

    //if(SDL_Init(SDL_INIT_EVERYTHING) != 0 /*0 is Success here*/)
    //{
    //    SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 INIT FAILED!");
    //    return EXIT_FAILURE;
    //}
    //
    //SDL_Window* Window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 200, NULL);
    //
    //if(!Window)
    //{
    //    SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "ERROR: SDL2 WINDOW CREATION FAILED!");
    //    return EXIT_FAILURE;
    //}
    //
    //bool bIsWindowOpen = true;
    //
    //while(bIsWindowOpen)
    //{
    //    SDL_Event Event;
	//    if(SDL_WaitEvent(&Event) > 0)
	//    {
	//	    if(Event.type == SDL_QUIT)
	//	    {
    //            bIsWindowOpen = false;
	//	    }
	//    }
    //}
    //
    //SDL_DestroyWindow(Window);
    //SDL_Quit();

    return EXIT_SUCCESS;
}
