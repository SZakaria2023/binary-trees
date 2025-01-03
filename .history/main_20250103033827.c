#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node* ls;
    struct node* rs;
} node;

node* create_node(int val){
    node* new = malloc(sizeof(node));
    new->value = val;
    new->ls = NULL;
    new->rs = NULL;
    return new;
}

node* add_node_in_bst(node* racine, int val){
    node* newNode = create_node(val);
    if(racine == NULL){
        racine = newNode;
    } else {
        node* temp1 = racine;
        node* temp2 = racine;
        while(temp1 != NULL){
            temp2 = temp1;
            if (temp1->value < val){
                temp1 = temp1->rs;
            } else {
                temp1 = temp1->ls;
            }
        }
        if(temp2->value > val){
            temp2->ls = newNode;
        } else {
            temp2->rs = newNode;
        }
    }
    return racine;
}

node* create_bst_from_table(int* table, int size){
    if(table == NULL){
        return NULL;
    } else {
        int i;
        node* racine = create_node(table[0]);
        for(i = 1; i < size; i++){
            add_node_in_bst(racine, table[i]);
        }
        return racine;
    }
}

node* search_for_node(node* racine, int val){
    if(racine == NULL) return NULL;
    if(racine->value == val) return racine;
    node* left_search = search_for_node(racine->ls, val);
    if(left_search != NULL){
        return left_search;
    } else {
        return search_for_node(racine->rs, val);
    }
}

node* copy_bst(node* racine){
    if (racine == NULL){
        return NULL;
    } else {
        node* copy = create_node(racine->value);
        copy->ls = copy_bst(racine->ls);
        copy->rs = copy_bst(racine->rs);
        return copy;
    }
}

node* integrate_bst_into_bst(node* bst1, node* bst2){
    if(bst1 == NULL) return bst2;
    if(bst2 == NULL) return bst1;
    bst1 = add_node_in_bst(bst1, bst2->value);
    bst1 = integrate_bst_into_bst(bst1, bst2->ls);
    bst1 = integrate_bst_into_bst(bst1, bst2->rs);
    return bst1;
}

node* delete_node_from_bst(node* racine, int val){
    if (racine == NULL) return NULL;
    if (val < racine->value){
        racine->ls = delete_node_from_bst(racine->ls, val);
    } else if (val > racine->value){
        racine->rs = delete_node_from_bst(racine->rs, val);
    } else {
        if (racine->ls == NULL){
            node* temp = racine->rs;
            free(racine);
            return temp;
        } else if (racine->rs == NULL){
            node* temp = racine->ls;
            free(racine);
            return temp;
        } else {
            node* temp = racine->rs;
            while (temp->ls != NULL){
                temp = temp->ls;
            }
            racine->value = temp->value;
            racine->rs = delete_node_from_bst(racine->rs, temp->value);
        }
    }
    return racine;
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius){
    int dx = radius - 1;
    int dy = 0;
    int err = dx - (radius << 1);
    int e2;

    while (dx >= dy){
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
        SDL_RenderDrawLine(renderer, x - dx, y - dy, x + dx, y - dy);
        SDL_RenderDrawLine(renderer, x - dy, y + dx, x + dy, y + dx);
        SDL_RenderDrawLine(renderer, x - dy, y - dx, x + dy, y - dx);

        if (err <= 0){
            dy += 1;
            err += (dy << 1) + 1;
        }
        if (err > 0){
            dx -= 1;
            err -= (dx << 1) + 1;
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background

    int table[] = {10, 20, 5, 15, 30, 25, 35};
    int size = sizeof(table) / sizeof(table[0]);

    node* bst = create_bst_from_table(table, size);

    SDL_RenderClear(renderer);

    // Drawing the BST
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the lines
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for the nodes

    // Draw the nodes and lines (for simplicity, this is an example to visualize nodes)
    // You can replace this with your own logic for positioning nodes in the window
    SDL_RenderFillCircle(renderer, 400, 200, 20); // Node at (400, 200)
    SDL_RenderFillCircle(renderer, 300, 300, 20); // Node at (300, 300)
    SDL_RenderFillCircle(renderer, 500, 300, 20); // Node at (500, 300)

    SDL_RenderDrawLine(renderer, 400, 200, 300, 300); // Line from (400, 200) to (300, 300)
    SDL_RenderDrawLine(renderer, 400, 200, 500, 300); // Line from (400, 200) to (500, 300)

    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

