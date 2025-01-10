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

