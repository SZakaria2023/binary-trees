
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
    if(racine=NULL)
    node* temp1=racine;
    node* temp2=racine;

    while(temp1!=NULL){
temp2=temp1;
        if (temp1->value<val){
            temp1=temp1->rs;
        }
        else{temp1=temp1->ls;}
    }
    node* newNode=create_node(val);
    if(temp2->ls==NULL){temp2->ls=newNode;}
    else{temp2->rs=newNode;}
    return racine;
}

node* create_bst_form_table(int* table,int size){
    int i;
    node* racine=create_node(table[0]);

    for(i=1;i<size;i++){
add_node_in_bst(racine,table[i]);
    }
    return racine;
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
}

node* delete_node_form_bst(node* racine,int val){
    node* position=search_for_node(racine,val);
    if(position!=NULL){
        if(position->ls=NULL && position->rs==NULL){free(position);}
else{

}
    }
    

}





int main(int argc, char* argv[]) {


    return 0;
}

