#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include 

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

node* create_bst_from_table(int* table, int size) {
    if (table == NULL) {
        return NULL;
    } else {
        int i;
        node* racine = create_node(table[0]);
        for (i = 1; i < size; i++) {
            add_node_in_bst(racine, table[i]);
        }
        return racine;
    }
}

node* search_for_node(node* racine, int val) {
    if (racine == NULL) {
        return NULL;
    } else {
        if (racine->value == val) {
            return racine;
        }
        node* left_search = search_for_node(racine->ls, val);
        if (left_search != NULL) {
            return left_search;
        } else {
            return search_for_node(racine->rs, val);
        }
    }
}

node* copy_bst(node* racine) {
    if (racine == NULL) {
        return NULL;
    } else {
        node* copy = create_node(racine->value);
        copy->ls = copy_bst(racine->ls);
        copy->rs = copy_bst(racine->rs);
        return copy;
    }
}

node* integrate_bst_into_bst(node* bst1, node* bst2) {
    if (bst1 == NULL) {
        return bst2;
    }
    if (bst2 == NULL) {
        return bst1;
    } else {
        bst1 = add_node_in_bst(bst1, bst2->value);
        bst1 = integrate_bst_into_bst(bst1, bst2->ls);
        bst1 = integrate_bst_into_bst(bst1, bst2->rs);
        return bst1;
    }
}

node* delete_node_from_bst(node* racine, int val) {
    if (racine == NULL) {
        return NULL;
    }
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

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y) {
    SDL_Color color = {0, 0, 0}; // Black
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_tree(SDL_Renderer* renderer, node* root, int x, int y, int offset) {
    if (root != NULL) {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", root->value);
        
        // Draw the node (circle)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        SDL_RenderFillCircle(renderer, x, y, 20);

        // Draw text inside the node
        render_text(renderer, font, buffer, x - 10, y - 10);

        // Draw the left and right child branches (lines)
        if (root->ls != NULL) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            SDL_RenderDrawLine(renderer, x, y, x - offset, y + 50);
            draw_tree(renderer, root->ls, x - offset, y + 50, offset / 2);
        }
        if (root->rs != NULL) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            SDL_RenderDrawLine(renderer, x, y, x + offset, y + 50);
            draw_tree(renderer, root->rs, x + offset, y + 50, offset / 2);
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("ShortBaby-Mg2w.ttf", 24);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }

    node* root = NULL;
    int choice, value;
    bool running = true;
    
    while (running) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);
        
        draw_tree(renderer, root, 400, 50, 100);
        SDL_RenderPresent(renderer);

        printf("Enter 1 to create a tree, 2 to add a node, 3 to search for a node, 4 to delete a node, 0 to exit: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter values for the tree (comma separated, ending with -1): ");
                int values[100], i = 0;
                while (1) {
                    scanf("%d", &value);
                    if (value == -1) break;
                    values[i++] = value;
                }
                root = create_bst_from_table(values, i);
                break;

            case 2:
                printf("Enter value to add: ");
                scanf("%d", &value);
                root = add_node_in_bst(root, value);
                break;

            case 3:
                printf("Enter value to search: ");
                scanf("%d", &value);
                node* found = search_for_node(root, value);
                if (found) {
                    printf("Node found with value: %d\n", found->value);
                } else {
                    printf("Node not found\n");
                }
                break;

            case 4:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                root = delete_node_from_bst(root, value);
                break;

            case 0:
                running = false;
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
