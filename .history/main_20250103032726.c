#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int value;
    struct node* ls;
    struct node* rs;
} node;

node* create_node(int val) {
    node* new = (node*)malloc(sizeof(node));
    new->value = val;
    new->ls = NULL;
    new->rs = NULL;
    return new;
}

node* add_node_in_bst(node* racine, int val) {
    if (!racine) return create_node(val);
    if (val < racine->value) racine->ls = add_node_in_bst(racine->ls, val);
    else racine->rs = add_node_in_bst(racine->rs, val);
    return racine;
}

node* find_min(node* racine) {
    while (racine && racine->ls) racine = racine->ls;
    return racine;
}

node* delete_node_form_bst(node* racine, int val) {
    if (!racine) return NULL;
    if (val < racine->value) racine->ls = delete_node_form_bst(racine->ls, val);
    else if (val > racine->value) racine->rs = delete_node_form_bst(racine->rs, val);
    else {
        if (!racine->ls) {
            node* temp = racine->rs;
            free(racine);
            return temp;
        } else if (!racine->rs) {
            node* temp = racine->ls;
            free(racine);
            return temp;
        }
        node* temp = find_min(racine->rs);
        racine->value = temp->value;
        racine->rs = delete_node_form_bst(racine->rs, temp->value);
    }
    return racine;
}

bool search_for_node(node* racine, int val) {
    if (!racine) return false;
    if (val == racine->value) return true;
    if (val < racine->value) return search_for_node(racine->ls, val);
    return search_for_node(racine->rs, val);
}

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void draw_bst(SDL_Renderer* renderer, TTF_Font* font, node* racine, int x, int y, int offset) {
    if (!racine) return;
    SDL_Color green = {0, 255, 0};
    SDL_Color red = {255, 0, 0};
    SDL_Color black = {0, 0, 0};

    draw_circle(renderer, x, y, 20, green);

    char value[10];
    sprintf(value, "%d", racine->value);
    render_text(renderer, font, value, x - 10, y - 10, black);

    if (racine->ls) {
        SDL_RenderDrawLine(renderer, x, y, x - offset, y + 50);
        draw_bst(renderer, font, racine->ls, x - offset, y + 50, offset / 2);
    }
    if (racine->rs) {
        SDL_RenderDrawLine(renderer, x, y, x + offset, y + 50);
        draw_bst(renderer, font, racine->rs, x + offset, y + 50, offset / 2);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1) return -1;

    SDL_Window* window = SDL_CreateWindow("Binary Search Tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("ShortBaby-Mg2w.ttf", 24);

    if (!window || !renderer || !font) return -1;

    SDL_Color white = {255, 255, 255};

    node* racine = NULL;
    racine = add_node_in_bst(racine, 50);
    racine = add_node_in_bst(racine, 30);
    racine = add_node_in_bst(racine, 70);
    racine = add_node_in_bst(racine, 20);
    racine = add_node_in_bst(racine, 40);
    racine = add_node_in_bst(racine, 60);
    racine = add_node_in_bst(racine, 80);

    bool running = true;
    SDL_Event event;

    while (running) {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
        SDL_RenderClear(renderer);

        draw_bst(renderer, font, racine, 400, 50, 150);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

