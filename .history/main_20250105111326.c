
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image Error: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Test SDL2_ttf: Render text
    TTF_Font *font = TTF_OpenFont("a.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Color color = {255, 255, 255}; // White text color
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, "SDL2_ttf Test", color);
    if (!text_surface) {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);

    // Test SDL2_image: Load an image (replace with an actual image path)
    SDL_Surface *image_surface = IMG_Load("White.jpg");
    if (!image_surface) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    // Main loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Test SDL2_gfx: Draw a circle
        filledCircleColor(renderer, 400, 300, 100, SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0, 255, 0));

        // Render text (SDL2_ttf)
        SDL_Rect text_rect = {200, 50, 400, 50};
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

        // Render image (SDL2_image)
        SDL_Rect image_rect = {200, 150, 400, 300};
        SDL_RenderCopy(renderer, image_texture, NULL, &image_rect);

        // Present the renderer
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Cap at ~60 FPS
    }

    // Clean up
    SDL_DestroyTexture(image_texture);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}