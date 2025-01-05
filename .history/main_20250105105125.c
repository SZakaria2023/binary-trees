
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node{
    int value;
    struct node* ls;
    struct node* rs;
}node;

node* create_node(int val){
    node* new=malloc(sizeof(node));
    new->value=val;
    new->ls=NULL;
    new->rs=NULL;
    return new;
}

node* add_node_in_bst(node* racine,int val){
    node* newNode=create_node(val);
    if(racine==NULL){
        racine=newNode;
    }
    else{
    node* temp1=racine;
    node* temp2=racine;

    while(temp1!=NULL){
temp2=temp1;
        if (temp1->value<val){
            temp1=temp1->rs;
        }
        else{temp1=temp1->ls;}
    }
    if(temp2->value>val){temp2->ls=newNode;}
    else{temp2->rs=newNode;}}
    return racine;
}

node* create_bst_form_table(int* table,int size){
    if(table==NULL){
        return NULL;
    }
    else{
    int i;
    node* racine=create_node(table[0]);

    for(i=1;i<size;i++){
add_node_in_bst(racine,table[i]);
    }
    return racine;}
}

node* search_for_node(node* racine,int val){
    if(racine==NULL){return NULL;}
    else{
        if(racine->value==val){return racine;}
        node* left_search=search_for_node(racine->ls,val);
        if(left_search!=NULL){
            return left_search;
        }

        else{ return search_for_node(racine->rs,val);}
    }

}
node* copy_bst(node* racine){
    if (racine==NULL){
        return NULL;
    }
    else{
        node* copy=create_node(racine->value);
        copy->ls=copy_bst( racine->ls);
        copy->rs=copy_bst( racine->rs);
        return copy;
    }
}
node* integrate_bst_into_bst(node* bst1,node* bst2){
    if(bst1==NULL){return bst2;}
    if(bst2==NULL){return bst1;}
    else{
    bst1 = add_node_in_bst(bst1, bst2->value);
    bst1 = integrate_bst_into_bst(bst1, bst2->ls);
    bst1 = integrate_bst_into_bst(bst1, bst2->rs);
    return bst1;}
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
void render_text(SDL_Renderer* renderer, const char* text, int x, int y) {
}

void render_circle(SDL_Renderer* renderer, int x, int y, int radius) {
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2 BST Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event e;
    node* root = NULL;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_a) {
                    root = add_node_in_bst(root, 10);
                }
                if (e.key.keysym.sym == SDLK_s) {
                    root = delete_node_form_bst(root, 10);
                }
                if (e.key.keysym.sym == SDLK_d) {
                    node* found = search_for_node(root, 10);
                    if (found != NULL) {
                        printf("Node found with value: %d\n", found->value);
                    } else {
                        printf("Node not found.\n");
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        render_circle(renderer, 400, 300, 50);

        render_text(renderer, "Press A to Add, S to Delete, D to Search", 20, 20);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
