#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node* ls;
    struct node* rs;
} node;

typedef struct coordonner {
    int* val;
    int* Y;
    int* X;
} coordonner;

struct node* create_node(int value) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->value = value;
    new_node->ls = NULL;
    new_node->rs = NULL;
    return new_node;
}
void draw_tree(node* root, int x, int y, SDL_Renderer* renderer, TTF_Font* font, coordonner* coords, int* index, int offsetX, int offsetY) {
    if (root == NULL) {
        return;
    }

    coords->val[*index] = root->value;
    coords->X[*index] = x + offsetX;
    coords->Y[*index] = y + offsetY;
    (*index)++;

    char valueStr[10];
    snprintf(valueStr, sizeof(valueStr), "%d", root->value);

    filledCircleRGBA(renderer, x + offsetX, y + offsetY, 20, 0, 0, 255, 255);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, valueStr, (SDL_Color){255, 255, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x + offsetX - 10, y + offsetY - 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    int xOffset = 100;
    int yOffset = 80;

    if (root->ls != NULL) {
        SDL_RenderDrawLine(renderer, x + offsetX, y + offsetY, x + offsetX - xOffset, y + offsetY + yOffset);
        draw_tree(root->ls, x + offsetX - xOffset, y + offsetY + yOffset, renderer, font, coords, index, offsetX, offsetY);
    }
    if (root->rs != NULL) {
        SDL_RenderDrawLine(renderer, x + offsetX, y + offsetY, x + offsetX + xOffset, y + offsetY + yOffset);
        draw_tree(root->rs, x + offsetX + xOffset, y + offsetY + yOffset, renderer, font, coords, index, offsetX, offsetY);
    }
}

void visualize_search(node* root, int target, coordonner* coords, SDL_Renderer* renderer, TTF_Font* font, int numNodes, int offsetX, int offsetY) {
    node* current = root;

    while (current != NULL) {
        int i = 0;
        while (i < numNodes && coords->val[i] != current->value) {
            i++;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        int temp = 0;
        draw_tree(root, 500, 50, renderer, font, coords, &temp, offsetX, offsetY);
        SDL_RenderPresent(renderer);

        filledCircleRGBA(renderer, coords->X[i], coords->Y[i], 35, 0, 255, 0, 100);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);

        if (current->value == target) {
            printf("Value %d found!\n", target);
            filledCircleRGBA(renderer, coords->X[i], coords->Y[i], 35, 0, 255, 0, 255);
            SDL_RenderPresent(renderer);
            break;
        }

        if (current->value > target) {
            current = current->ls;
        } else {
            current = current->rs;
        }
    }

    if (current == NULL) {
        printf("Value %d not found!\n", target);
    }
}

bool is_mouse_over_node(int mouseX, int mouseY, int nodeX, int nodeY) {
    int radius = 20;
    int dx = mouseX - nodeX;
    int dy = mouseY - nodeY;
    return (dx * dx + dy * dy <= radius * radius);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("TTF Initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    int numNodes;
    printf("Enter the number of nodes: ");
    scanf("%d", &numNodes);

    int* values = (int*)malloc(sizeof(int) * numNodes);
    printf("Enter the node values:\n");
    for (int i = 0; i < numNodes; i++) {
        scanf("%d", &values[i]);
    }

    node* root = create_bst_from_table(values, numNodes);

    int target;
    printf("Enter the value to search for: ");
    scanf("%d", &target);

    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("Arial.ttf", 16);
    if (!font) {
        printf("Font loading failed: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    coordonner coords;
    coords.val = (int*)malloc(sizeof(int) * numNodes);
    coords.X = (int*)malloc(sizeof(int) * numNodes);
    coords.Y = (int*)malloc(sizeof(int) * numNodes);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    int index = 0;
    int offsetX = 0, offsetY = 0;
    draw_tree(root, 500, 50, renderer, font, &coords, &index, offsetX, offsetY);
    SDL_RenderPresent(renderer);

    visualize_search(root, target, &coords, renderer, font, numNodes, offsetX, offsetY);
    
    bool continueRunning = true;
    bool dragging = false;
    int prevMouseX = 0, prevMouseY = 0;
    node* clickedNode = NULL;
    SDL_Event event;

    while (continueRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                continueRunning = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < numNodes; i++) {
                        if (is_mouse_over_node(event.button.x, event.button.y, coords.X[i] + offsetX, coords.Y[i] + offsetY)) {
                            dragging = true;
                            prevMouseX = event.button.x;
                            prevMouseY = event.button.y;
                            clickedNode = root;
                            break;
                        }
                    }
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = false;
                    clickedNode = NULL;
                }
            }

            if (event.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    offsetX += mouseX - prevMouseX;
                    offsetY += mouseY - prevMouseY;

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    int temp = 0;
                    draw_tree(root, 500, 50, renderer, font, &coords, &temp, offsetX, offsetY);
                    SDL_RenderPresent(renderer);

                    prevMouseX = mouseX;
                    prevMouseY = mouseY;
                }
            }
        }
    }

    free(values);
    free(coords.val);
    free(coords.X);
    free(coords.Y);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
