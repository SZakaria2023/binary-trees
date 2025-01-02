
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
int main(int argc, char* argv[]) {
    SDL_Renderer* renderer=NULL;
    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(800,800,0,&window,&renderer);

    SDL_SetRenderDrawColor(rendere0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(255,255,255,255);
    s

    return 0;
}

