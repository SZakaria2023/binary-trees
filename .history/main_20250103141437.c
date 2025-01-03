#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Structure for a Binary Search Tree node
typedef struct node {
    int value;
    struct node* ls;
    struct node* rs;
} node;

// Create a new node
node* create_node(int val) {
    node* new = malloc(sizeof(node));
    new->value = val;
    new->ls = NULL;
    new->rs = NULL;
    return new;
}

// Add a node to the BST
node* add_node_in_bst(node* root, int val) {
    if (root == NULL) {
        return create_node(val);
    }
    if (val < root->value) {
        root->ls = add_node_in_bst(root->ls, val);
    } else {
        root->rs = add_node_in_bst(root->rs, val);
    }
    return root;
}

// Function to render a filled circle
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int dx = radius - 1;
    int dy = 0;
    int err = dx - (radius << 1);
    while (dx >= dy) {
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
        SDL_RenderDrawLine(renderer, x - dx, y - dy, x + dx, y - dy);
        SDL_RenderDrawLine(renderer, x - dy, y + dx, x + dy, y + dx);
        SDL_RenderDrawLine(renderer, x - dy, y - dx, x + dy, y - dx);
        if (err <= 0) {
            dy++;
            err += (dy << 1) + 1;
        }
        if (err > 0) {
            dx--;
            err -= (dx << 1) + 1;
        }
    }
}

// Function to render the tree
void render_tree(SDL_Renderer* renderer, node* root, int x, int y, int dx, TTF_Font* font) {
    if (root == NULL) return;

    // Draw the node circle
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    SDL_RenderFillCircle(renderer, x, y, 20);

    // Render the value of the node
    char value_str[10];
    snprintf(value_str, sizeof(value_str), "%d", root->value);
    SDL_Color textColor = {0, 0, 0, 255}; // Black
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, value_str, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x - textSurface->w / 2, y - textSurface->h / 2, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Draw the connections to child nodes
    if (root->ls != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderDrawLine(renderer, x, y, x - dx, y + 60);
        render_tree(renderer, root->ls, x - dx, y + 60, dx / 2, font);
    }
    if (root->rs != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderDrawLine(renderer, x, y, x + dx, y + 60);
        render_tree(renderer, root->rs, x + dx, y + 60, dx / 2, font);
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() != 0) {
        printf("SDL_ttf Initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Create the window and renderer
    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the font
    TTF_Font* font = TTF_OpenFont("ShortBaby-Mg2w.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize the BST
    node* bst = NULL;

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_a) {
                    bst = add_node_in_bst(bst, rand() % 100); // Add random values for demonstration
                }
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        SDL_RenderClear(renderer);

        // Render the tree
        render_tree(renderer, bst, 400, 50, 150, font);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
