
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>


int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window *window = SDL_CreateWindow("SDL_gfx Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create an SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the renderer draw color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(renderer);

    // Draw a line
    lineRGBA(renderer, 50, 50, 200, 200, 255, 0, 0, 255);

    // Draw a rectangle
    rectangleRGBA(renderer, 220, 50, 370, 200, 0, 255, 0, 255);

    // Draw a filled circle
    filledCircleRGBA(renderer, 320, 240, 100, 0, 0, 255, 255);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Wait for 5 seconds
    SDL_Delay(5000);

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}