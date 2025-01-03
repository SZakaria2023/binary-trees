#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
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

node* delete_node_form_bst(node* racine, int val) {
    if (racine == NULL) {
        return NULL;
    }
    if (val < racine->value) {
        racine->ls = delete_node_form_bst(racine->ls, val);
    } else if (val > racine->value) {
        racine->rs = delete_node_form_bst(racine->rs, val);
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
            racine->rs = delete_node_form_bst(racine->rs, temp->value);
        }
    }
    return racine;
}

node* search_for_node(node* racine, int val) {
    if (racine == NULL) {
        return NULL;
    }
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

void handle_input(SDL_Event e, node** bst, char* value_str) {
    static int input_value = 0;

    if (e.type == SDL_TEXTINPUT) {
        strcat(value_str, e.text.text); // Add the input text
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_A) {  // Press 'a' or 'A' to add node
            if (strlen(value_str) > 0) {
                sscanf(value_str, "%d", &input_value);
                *bst = add_node_in_bst(*bst, input_value);
                value_str[0] = '\0'; // Clear the text field
            }
        }

        if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_D) {  // Press 'd' or 'D' to delete node
            if (strlen(value_str) > 0) {
                sscanf(value_str, "%d", &input_value);
                *bst = delete_node_form_bst(*bst, input_value);
                value_str[0] = '\0'; // Clear the text field
            }
        }

        if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_S) {  // Press 's' or 'S' to search node
            if (strlen(value_str) > 0) {
                sscanf(value_str, "%d", &input_value);
                node* found = search_for_node(*bst, input_value);
                if (found) {
                    printf("Node %d found!\n", input_value);
                } else {
                    printf("Node %d not found.\n", input_value);
                }
                value_str[0] = '\0'; // Clear the text field
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Binary Search Tree Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_StartTextInput();

    bool running = true;
    SDL_Event e;
    node* bst = NULL;  // Initializing the BST to NULL
    char input_text[100] = "";

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            handle_input(e, &bst, input_text);
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Here you can visualize the tree by rendering it if you need to
        // (I won't include the visualization code since it's not the focus here)

        // Update the screen
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Delay for ~60fps
    }

    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
