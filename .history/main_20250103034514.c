#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node* ls;
    struct node* rs;
} node;

node* create_node(int val) {
    node* new = malloc(sizeof(node));
    new->value = val;
    new->ls = NULL;
    new->rs = NULL;
    return new;
}

node* add_node_in_bst(node* racine, int val) {
    node* newNode = create_node(val);
    if (racine == NULL) {
        racine = newNode;
    } else {
        node* temp1 = racine;
        node* temp2 = racine;
        while (temp1 != NULL) {
            temp2 = temp1;
            if (temp1->value < val) {
                temp1 = temp1->rs;
            } else {
                temp1 = temp1->ls;
            }
        }
        if (temp2->value > val) {
            temp2->ls = newNode;
        } else {
            temp2->rs = newNode;
        }
    }
    return racine;
}

node* delete_node_from_bst(node* racine, int val) {
    if (racine == NULL) return NULL;
    if (val < racine->value) {
        racine->ls = delete_node_from_bst(racine->ls, val);
    } else if (val > racine->value) {
        racine->rs = delete_node_from_bst(racine->rs, val);
    } else {
        if (racine->ls == NULL) {
            node* temp = racine->rs;
            free(racine);
            return temp;
        } else if (racine->rs == NULL) {
            node* temp = racine->ls;
            free(racine);
            return temp;
        } else {
            node* temp = racine->rs;
            while (temp->ls != NULL) {
                temp = temp->ls;
            }
            racine->value = temp->value;
            racine->rs = delete_node_from_bst(racine->rs, temp->value);
        }
    }
    return racine;
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int dx = radius - 1;
    int dy = 0;
    int err = dx - (radius << 1);
    int e2;

    while (dx >= dy) {
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
        SDL_RenderDrawLine(renderer, x - dx, y - dy, x + dx, y - dy);
        SDL_RenderDrawLine(renderer, x - dy, y + dx, x + dy, y + dx);
        SDL_RenderDrawLine(renderer, x - dy, y - dx, x + dy, y - dx);

        if (err <= 0) {
            dy += 1;
            err += (dy << 1) + 1;
        }
        if (err > 0) {
            dx -= 1;
            err -= (dx << 1) + 1;
        }
    }
}

void render_tree(SDL_Renderer* renderer, node* root, int x, int y, int dx, TTF_Font* font) {
    if (root == NULL) return;

    // Draw the node circle (Green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    SDL_RenderFillCircle(renderer, x, y, 20);

    // Render the value of the node (Black text)
    char value_str[20];
    snprintf(value_str, sizeof(value_str), "%d", root->value);

    SDL_Color textColor = {0, 0, 0}; // Black text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, value_str, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x - 10, y - 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Draw lines to the left and right children (Red)
    if (root->ls != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_RenderDrawLine(renderer, x, y, x - dx, y + 60);
        render_tree(renderer, root->ls, x - dx, y + 60, dx / 2, font);
    }

    if (root->rs != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_RenderDrawLine(renderer, x, y, x + dx, y + 60);
        render_tree(renderer, root->rs, x + dx, y + 60, dx / 2, font);
    }
}

void handle_input(SDL_Event e, node** bst, SDL_Renderer* renderer, TTF_Font* font) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_a) {  // Press 'a' to add node
            int val = 0; // Hardcoded for simplicity
            *bst = add_node_in_bst(*bst, val);
        } else if (e.key.keysym.sym == SDLK_d) {  // Press 'd' to delete node
            int val = 0; // Hardcoded for simplicity
            *bst = delete_node_from_bst(*bst, val);
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("ShortBaby-Mg2w.ttf", 24);
    if (font == NULL) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(renderer);

    node* bst = NULL;  // Initializing the BST to NULL

    // Main Loop
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            handle_input(e, &bst, renderer, font);
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Render the tree
        render_tree(renderer, bst, 400, 50, 150, font);

        // Update the screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Delay for ~60fps
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
