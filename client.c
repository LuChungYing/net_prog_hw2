#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

char oigindata[1000000];
char afterdata[1000000];
int main() {
  struct sockaddr_in info;

  static unsigned int fd;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // no AF_LOCAL
    perror("creat socket error.\n");
  char *buf = "123";
  bzero(&info, sizeof(info));
  while (1) {
    char *tmp = malloc(100);
    scanf("%s", tmp);
    if (!strcmp(tmp, "goodbye")) {
      printf("See you next time. ");
      break;
    } else if (!strcmp(tmp, "connect")) {
      char ip[100], port[100];
      scanf(" %s %s", ip, port);
      info.sin_family = AF_INET;
      info.sin_addr.s_addr = inet_addr(ip);
      info.sin_port = htons(atoi(port));
      if (connect(fd, (struct sockaddr *)&info, sizeof(info)) == -1)
        perror("connect error");
      printf("The server with IP “%s” has accepted your connection. ", ip);
    } else if (!strcmp(tmp, "upload")) {
      FILE *file;
      char filename[100];
      scanf("%s", filename);
      file = fopen(filename, "rb");
      int num = 0;
      bzero(filedata, sizeof(filedata));
      while (!feof(file)) {
        fread(filedata + num, sizeof(char), 1, file);
        ++num;
      }
      for (int i = 0; i < num; ++i) {
        printf("%c", filedata[i]);
      }
      afterdata = compress(filedata);
      printf("Original file length: %d bytes, compressed file length: 23,768 "
             "bytes (ratio: 16.95%)",
             num);
    } else {
    }
    // send(fd , buf, sizeof(buf), MSG_CONFIRM);
  }
}
