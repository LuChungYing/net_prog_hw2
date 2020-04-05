#include <arpa/inet.h>
#include <fcntl.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
char data[1024];
typedef struct treenode {
  struct treenode *left;
  struct treenode *right;
  int ASCII;
} node_t;
node_t *root;

void buildtree(int, unsigned char *, int);
int used_ASCII[256];
size_t p_size;
size_t o_size;
int main() {
  struct sockaddr_in server_info, client_info;
  int addrlen = sizeof(client_info);
  static unsigned int fd, client_fd;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // no AF_LOCAL
    perror("creat socket error.\n");
  char *buf = malloc(sizeof(4));
  bzero(&server_info, sizeof(server_info));

  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = htonl(INADDR_ANY);
  server_info.sin_port = htons(8700);

  if (bind(fd, (struct sockaddr *)&server_info, sizeof(server_info)) == -1)
    perror("bind error");
  listen(fd, 5);
  printf("Listening...\n");

  while (1) {

    client_fd =
        accept(fd, (struct sockaddr *)&client_info, &addrlen); // three way

    printf("A client “%s” has connected via port %d using SOCK_STREAM (TCP)\n",
           inet_ntoa(client_info.sin_addr), client_info.sin_port);
    fflush(stdout);
    int s;
    while ((s = read(client_fd, data, sizeof(data))) > 0) {
      FILE *ptr = fopen("receive", "wb");
      fwrite(data, s, 1, ptr);
      memcpy(&p_size, data, sizeof(size_t));
      p_size = ntohl(p_size);
      p_size -= s;
      memcpy(&o_size, data + 4, sizeof(size_t));
      o_size = ntohl(o_size);
      char filename[255];
      memcpy(filename, data + 8, 255); // 4(p_size) +4(o_size)
      printf("filename = %s\n", filename);
      unsigned char lencode;
      unsigned char code[8];
      for(int i=0;i<256;i++){
          if (data[263+i/8] & 128 >> i%8)
              used_ASCII[i] = 1; 
      }
      int codeindex = 295;
      root = (node_t *)calloc(sizeof(node_t), 1);
      for (int i = 0; i < 256; i++) {
        if (used_ASCII[i]) {
          lencode = data[codeindex++];
          memcpy(code, &data[codeindex], ceil(lencode / 8.0));
          codeindex += ceil(lencode / 8.0);
          buildtree(i, code, lencode);
        }
      }
      strcat(filename, ".code");
      int ffd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
      node_t *current = root;
      for (int i = codeindex; i < s; i++) {
        for (int j = 0; j < 8; j++) {
          if (data[i] & 128 >> j) {
            current = current->right;
          } else {
            current = current->left;
          }
          if (!(current->right) && !(current->left)) {
            unsigned char t = current->ASCII;

            write(ffd, &t, 1);
            current = root;
            o_size--;
            if (!o_size)
              break;
          }
        }
      }
      while (p_size > 0) {
        s = read(client_fd, data, 1024 < p_size ? 1024 : p_size);
        p_size -= s;
        fwrite(data, s, 1, ptr);
        if (s <= 0)
          break;
        for (int i = 0; i < s; i++) {
          for (int j = 0; j < 8; j++) {
            if (data[i] & 128 >> j) {
              current = current->right;
            } else {
              current = current->left;
            }
            if (!(current->right) && !(current->left)) {
              unsigned char t = current->ASCII;
              write(ffd, &t, 1);
              current = root;
              o_size--;
              if (!o_size)
                break;
            }
          }
        }
      }
      close(ffd);
      fclose(ptr);
    }
  }
}
void buildtree(int ASCII, unsigned char *code, int depth) {
  node_t *current = root;
  for (int i = 0; i < depth; ++i) {
    if (code[(i / 8)] & 128 >> (i % 8)) {
      if (!current->right) {
        current->right = (node_t *)calloc(sizeof(node_t), 1);
      }
      current = current->right;
    } else {
      if (!current->left) {
        current->left = (node_t *)calloc(sizeof(node_t), 1);
      }
      current = current->left;
    }
  }
  current->ASCII = ASCII;
}
