
#include <SDL2/SDL.h>
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

node* add_node_in_bts(node* racine,int val){
    node* temp=racine;
    while(){
        if (temp->value<=val){
            temp=temp->rs;
        }
        else{temp=temp->ls}
    }
    if
    


    return racine;
}






int main(int argc, char* argv[]) {


    return 0;
}

