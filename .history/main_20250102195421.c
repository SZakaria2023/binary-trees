
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

node* add_node_in_bst(node* racine,int val){
    node* temp1=racine;
    node* temp2=racine;

    while(temp1!=NULL){
temp2=temp1;
        if (temp1->value<=val){
            temp1=temp1->rs;
        }
        else{temp1=temp1->ls;}
    }
    node* newNode=create_node(val);
    if(temp2->ls=NULL){temp2->ls=newNode;}
    else{temp2->rs=newNode;}
    return racine;
}

node* create_bst_form_table(int* table,int size){
    int i;
    node* racine=create_node(table[0]);

    for(i==1;i<size;i++){
add
    }
}






int main(int argc, char* argv[]) {


    return 0;
}

