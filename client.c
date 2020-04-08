#include "huffman.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
int main() {
  struct sockaddr_in info;

  unsigned int fd;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // no AF_LOCAL
    perror("creat socket error.\n");

  bzero(&info, sizeof(info));
  while (1) {
    char tmp[20];
    scanf("%s", tmp);
    if (!strcmp(tmp, "goodbye")) {
      printf("See you next time.\n");
      break;
    } else if (!strcmp(tmp, "connect")) {
      char ip[100], port[100];
      scanf(" %s %s", ip, port);
      info.sin_family = AF_INET;
      info.sin_addr.s_addr = inet_addr(ip);
      info.sin_port = htons(atoi(port));
      if (connect(fd, (struct sockaddr *)&info, sizeof(info)) == -1)
        perror("connect error");
      else
        printf("The server with IP “%s” has accepted your connection. \n", ip);
    } else if (!strcmp(tmp, "upload")) {
      FILE *file;
      char filename[100];

      scanf(" %s", filename);
      file = fopen(filename, "rb");
      int num = 0;
      bzero(origindata, sizeof(origindata));

      while (fread(origindata + num, sizeof(char), 1, file) > 0) {
        ++num;
      }
      size = num;
      int aftersize = compress(fd, filename);
      printf(
          "Original file length: %d bytes, compressed file length: %d  \n",
          num,aftersize);
    } else {
    }
    // send(fd , buf, sizeof(buf), MSG_CONFIRM);
  }
}

int compress(int fd, char *filename) {
  find_frequency();
  // do huffman

  while (1) {

    if (listsize < 2) {
      treeroot = head->ptr;
      break;
    }

    listnode_t *n = deletelist(head);

    listnode_t *new = (listnode_t *)malloc(sizeof(listnode_t));
    new->ptr = insert_huffmantree(n->ptr, n->next->ptr);
    free(n->next);
    free(n);
    new->num = new->ptr->frequency;
    insert_linkedlist(new);
  }
  unsigned char c[8] = {0};
  dfs_coding(treeroot, c, 0);
  return  coding(fd, filename);
}
