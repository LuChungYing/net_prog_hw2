#include <stdio.h>
unsigned char origindata[1000000];
unsigned char afterdata[1000000];

size_t size;

typedef struct HuffmanNode {
  int frequency;
  int ASCII;
  struct HuffmanNode *left;
  struct HuffmanNode *right;
} node_t;

typedef struct {
  unsigned char lencode;
  unsigned char code[8]; // 8 byte
} code_t;

node_t *treeroot;

typedef struct listNode {
  int num;
  int ASCII;
  node_t *ptr;
  struct listNode *next;
} listnode_t;

unsigned int listsize;

listnode_t *head;
int ASCII_frequency[256];
code_t record[256];

void find_frequency();
void sort();
listnode_t *deletelist();
node_t *insert_huffmantree(node_t *first, node_t *second);
void insert_linkedlist(listnode_t *New);

void dfs_coding(node_t *root, unsigned char *Code, unsigned char depth);

int min(int, int);
int coding(int, char *);
int compress(int fd, char *filename);
