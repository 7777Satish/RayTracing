#include <stdio.h>
#include "renderer.h"
#include "utils.h"

int main(){
    init();
    
    renderBG();
    SDL_RenderPresent(renderer);
    render();
    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
            }
        }

        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderClear(renderer);
        // render();
        // SDL_RenderPresent(renderer);
        // SDL_Delay(1000/60);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}