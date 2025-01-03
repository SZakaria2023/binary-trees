#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int value;
    struct node *ls; // Left subtree
    struct node *rs; // Right subtree
} node;

node* add_node_in_bst(node* node_ptr, int val);
node* delete_node_from_bst(node* node_ptr, int val);
node* search_for_node(node* node_ptr, int val);
node* create_node(int val);
void render_tree(SDL_Renderer* renderer, node* node_ptr, int x, int y, int dx, TTF_Font* font);
void render_instructions(SDL_Renderer* renderer, TTF_Font* font);
void handle_key_input(SDL_Event event, node** bst, int* current_value);

// Create a new node
node* create_node(int val) {
    node* new_node = malloc(sizeof(node));
    new_node->value = val;
    new_node->ls = NULL;
    new_node->rs = NULL;
    return new_node;
}

// Add a node to the BST
node* add_node_in_bst(node* node_ptr, int val) {
    if (node_ptr == NULL) return create_node(val);
    if (val < node_ptr->value) {
        node_ptr->ls = add_node_in_bst(node_ptr->ls, val);
    } else if (val > node_ptr->value) {
        node_ptr->rs = add_node_in_bst(node_ptr->rs, val);
    }
    return node_ptr;
}

// Remove a node from the BST
node* delete_node_from_bst(node* node_ptr, int val) {
    if (node_ptr == NULL) return NULL;

    if (val < node_ptr->value) {
        node_ptr->ls = delete_node_from_bst(node_ptr->ls, val);
    } else if (val > node_ptr->value) {
        node_ptr->rs = delete_node_from_bst(node_ptr->rs, val);
    } else {
        if (node_ptr->ls == NULL) {
            node* temp = node_ptr->rs;
            free(node_ptr);
            return temp;
        } else if (node_ptr->rs == NULL) {
            node* temp = node_ptr->ls;
            free(node_ptr);
            return temp;
        }
        node* temp = node_ptr->rs;
        while (temp->ls != NULL) temp = temp->ls;
        node_ptr->value = temp->value;
        node_ptr->rs = delete_node_from_bst(node_ptr->rs, temp->value);
    }
    return node_ptr;
}

// Search for a node in the BST
node* search_for_node(node* node_ptr, int val) {
    if (node_ptr == NULL || node_ptr->value == val) return node_ptr;
    if (val < node_ptr->value) return search_for_node(node_ptr->ls, val);
    return search_for_node(node_ptr->rs, val);
}

// Render the BST nodes
void render_tree(SDL_Renderer* renderer, node* node_ptr, int x, int y, int dx, TTF_Font* font) {
    if (node_ptr == NULL) return;

    // Draw the node
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for node
    SDL_Rect rect = {x - 20, y - 20, 40, 40};
    SDL_RenderFillRect(renderer, &rect);

    // Render node value
    char value[10];
    snprintf(value, sizeof(value), "%d", node_ptr->value);
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderText_Solid(font, value, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {x - surface->w / 2, y - surface->h / 2, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw lines to children
    if (node_ptr->ls != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderDrawLine(renderer, x, y, x - dx, y + 60);
        render_tree(renderer, node_ptr->ls, x - dx, y + 60, dx / 2, font);
    }
    if (node_ptr->rs != NULL) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderDrawLine(renderer, x, y, x + dx, y + 60);
        render_tree(renderer, node_ptr->rs, x + dx, y + 60, dx / 2, font);
    }
}

// Render instructions on how to interact with the tree
void render_instructions(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {0, 0, 0};  // Black for text

    // Instructions text
    const char* instructions[] = {
        "Press 'A' to add a node",
        "Press 'D' to delete a node",
        "Press 'S' to search for a node",
        "Press '1' to input value for current action"
    };

    // Render each instruction
    for (int i = 0; i < 4; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, instructions[i], textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {10, 10 + i * 40, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

// Handle key input to update the value or perform actions
void handle_key_input(SDL_Event event, node** bst, int* current_value) {
    if (event.key.keysym.sym == SDLK_a) { // Add node
        *bst = add_node_in_bst(*bst, *current_value);
    } else if (event.key.keysym.sym == SDLK_d) { // Delete node
        *bst = delete_node_from_bst(*bst, *current_value);
    } else if (event.key.keysym.sym == SDLK_s) { // Search node
        node* search_result = search_for_node(*bst, *current_value);
        if (search_result) {
            printf("Node with value %d found!\n", *current_value);
        } else {
            printf("Node with value %d not found.\n", *current_value);
        }
    } else if (event.key.keysym.sym == SDLK_1) { // Input value for the current action
        *current_value = (*current_value + 1) % 10;  // Just an example, incrementing value.
        printf("Current value: %d\n", *current_value); // Display value in console
    }
}

// Main function
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("BST Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("ShortBaby-Mg2w.ttf", 24);

    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return -1;
    }

    node* bst = NULL; // Initialize BST
    int current_value = 1; // Default value for actions

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                handle_key_input(event, &bst, &current_value);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        // Render instructions at the top
        render_instructions(renderer, font);

        // Render the BST
        render_tree(renderer, bst, 400, 50, 200, font);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // To limit the frame rate
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

