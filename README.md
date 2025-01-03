#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}


Node* insert(Node* root, int value) {
    if (root == NULL) return createNode(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);
    return root;
}


void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int dx = radius - 1;
    int dy = 0;
    int err = dx - (radius << 1);

    while (dx >= dy) {
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        SDL_RenderDrawPoint(renderer, x + dy, y + dx);
        SDL_RenderDrawPoint(renderer, x - dy, y + dx);
        SDL_RenderDrawPoint(renderer, x - dx, y + dy);
        SDL_RenderDrawPoint(renderer, x - dx, y - dy);
        SDL_RenderDrawPoint(renderer, x - dy, y - dx);
        SDL_RenderDrawPoint(renderer, x + dy, y - dx);
        SDL_RenderDrawPoint(renderer, x + dx, y - dy);

        if (err <= 0) {
            dy++;
            err += dy << 1;
        }
        if (err > 0) {
            dx--;
            err -= dx << 1;
        }
    }
}

void drawTree(SDL_Renderer* renderer, Node* root, int x, int y, int spacing) {
    if (root == NULL) return;

    int nodeRadius = 20;

    if (root->left != NULL) {
        SDL_RenderDrawLine(renderer, x, y, x - spacing / 2, y + 100);
        drawTree(renderer, root->left, x - spacing / 2, y + 100, spacing / 2);
    }
    if (root->right != NULL) {
        SDL_RenderDrawLine(renderer, x, y, x + spacing / 2, y + 100);
        drawTree(renderer, root->right, x + spacing / 2, y + 100, spacing / 2);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Couleur bleue
    drawCircle(renderer, x, y, nodeRadius);


}


int main(int argc, char* argv[]) {

    int n;
    printf("Entrez le nombre d'éléments dans le tableau : ");
    scanf("%d", &n);

    int* values = (int*)malloc(n * sizeof(int));
    printf("Entrez les %d valeurs séparées par un espace :\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &values[i]);
    }

 
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }
    free(values);


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;
    while (running) {
        // Gérer les événements SDL
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

  
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur noire pour les branches
        drawTree(renderer, root, 400, 50, 200);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
