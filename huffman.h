#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct HuffmanNode
{
    int frequency;
    int ASCCI;
    char code; //1 byte
    node *left;
    node *right;
}node_t;

node *treeroot;

typedef struct listNode
{
    int num;
    node *ptr;
    linknode *next;
}listnode_t_t;

unsigned int listsize; 

listnode_t *head;
int ASCCI_frequency[256];
char table[256];


void find_frequcency(char* file);
void sort();
listnode_t* deletelist();
node* insert_huffmantree(node* first, node* second);
void insert_linkedlist(node* New);

void dfs_codeing(node *root);


char* coding();
char* compress(char *file);

