#include "huffman.h"

void find_frequcency(char* file)
{
    for(int i=0; file[i]!= EOF; ++i){
        ++ASCCI_frequency[ file[i] ];
    }
    
    for(int i=0; i<256; ++i){
        if(ASCCI_frquency[i]!=0){
            listnode_t *new =(listnode_t*)malloc(sizeof(listnode_t));
            insert_linkedlist(new);
        }
    }

}
listnode_t* deletelist(){
    listnode_t **tmp = &head;

    if (!head) 
        return;


}
node* insert_huffmantree(node* first, node* second);
void insert_linkedlist(node* New){

    listnode_t **tmp = &head;

    if (!head) 
        return;

    listnode_t *new = (listnode_t*)malloc(sizeof(listnode));
    new = New;

    if (!head->next){
        head = new;
        head->next = NULL;
        return;
    }
    for (; !*tmp->next || *tmp->next->num > New->num; tmp = &(*tmp->next) ); //last one  or after this is bigger
   

    listnode_t *t = *tmp->next;
    *tmp->next = new;
    new->next = t;
    

}

void dfs_codeing(node *root);
char* coding();

char* compress(char *file)
{
    find_frequency(file);
    listsize = 0;

    sort();
    
    //do huffman
    while(1){

        if (listsize < 2) 
        {
            treeroot = head->ptr;
            break;
        }

        listnode_t *n = deletelist(head);

        listnode_t *new = (*listnode_t) malloc(sizeof(listnode_t));
        new->ptr = insert_huffmantree(n->ptr, n->next->ptr);
        insert_linkedlist(new);

    }

    dfs_coding(treeroot);

    return coding(file);

}


