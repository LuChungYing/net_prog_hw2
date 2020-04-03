#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
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
  client_fd = accept(fd, (struct sockaddr *)&client_info, &addrlen);
  recv(client_fd, buf, sizeof(buf), 0);
  printf("%s", buf);
}
