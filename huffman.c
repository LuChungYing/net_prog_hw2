#include "huffman.h"

void find_frequency() {
  for (int i = 0; i < size; ++i) {
    ++ASCII_frequency[origindata[i]];
  }

  for (int i = 0; i < 256; ++i) {
    if (ASCII_frequency[i] != 0) {
      listnode_t new;
      new.num = ASCII_frequency[i];
      new.ASCII = i;

      new.ptr = (node_t *)calloc(1,sizeof(node_t));
      (new.ptr)->frequency = new.num;
      (new.ptr)->ASCII = i;

      insert_linkedlist(&new);
    }
  }
  listnode_t *tmp = head;

}

listnode_t *deletelist() {
  listnode_t *tmp = head;
  head = tmp->next->next;
  tmp->next->next = NULL;
  listsize -= 2;
  return tmp;
}

node_t *insert_huffmantree(node_t *first, node_t *second) {

  node_t *new = (node_t *)calloc(1,sizeof(node_t));

  new->left = first;
  new->right = second;

  new->frequency = first->frequency + second->frequency;

  return new;
}

void insert_linkedlist(listnode_t *New) {

  listnode_t *tmp = head;

  listnode_t *new = (listnode_t *)malloc(sizeof(listnode_t));
  *new = *New;

  if (!head) {
    head = new;
    head->next = NULL;
    listsize = 1;
    return;
  }

  if (head->num > new->num) {
    listnode_t *t = head;
    head = new;
    new->next = t;
    listsize++;
    return;
  }
  for (; tmp->next && tmp->next->num <= new->num; tmp = tmp->next) {
  }

  listsize++;
  listnode_t *t = tmp->next;
  tmp->next = new;
  new->next = t;
}

void dfs_coding(node_t *root, unsigned char* Code, unsigned char depth) {

    if(!root->left && !root->right){
        memcpy(record[root->ASCII].code,Code,8);
        record[root->ASCII].lencode = depth;
        printf("%c: ",root->ASCII);
        for(int i=0;i<depth;i++){
            if(Code[i/8] & ( (128 >> i%8)))
                printf("1");
            else
                printf("0");
        }
        printf("\n");
        return;
    }
    Code[depth/8] = Code[depth/8] & ( ~(128 >> depth%8) );
    dfs_coding(root->left,Code, depth+1);

    Code[depth/8] = Code[depth/8] | ( 128 >> depth%8) ;
    dfs_coding(root->right,Code , depth+1);


}
int coding() {
    unsigned char t = 0; int offset=0; int after_index=0;
    for(int i=0;i<size;++i){
        code_t tmp = record[origindata[i]];
        for(int j=0; j<tmp.lencode;++j){
            if(tmp.code[j/8] & (128 >> j%8) )
                t = t | (128>>offset);
            offset++;
            if(offset >= 8){
                afterdata[after_index++] = t;
                t = 0;
                offset = 0;
            }
         }
    }
    FILE *ptr = fopen("after","wb");
    if(!ptr) printf("open error\n");
    afterdata[after_index++] = t;  
    fwrite(afterdata,sizeof(char),after_index,ptr);
    fclose(ptr);
    return after_index;
}

