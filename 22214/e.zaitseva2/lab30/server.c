#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
  unlink("./socket");

  int server_socket, client_socket;
  server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Error creating socket");
    exit(1);
  }
  struct sockaddr_un serv_addr;
  memset(&serv_addr, 0, sizeof(struct sockaddr_un));
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, "./socket");

  if (bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ==
      -1) {
    close(server_socket);
    perror("Error binding socket to file");
    exit(1);
  }
  if (listen(server_socket, 0) == -1) {
    close(server_socket);
    unlink("./socket");
    perror("Error listenning");
    exit(1);
  }
  client_socket = accept(server_socket, NULL, NULL);
  if (client_socket == -1) {
    close(server_socket);
    unlink("./socket");
    perror("Error accepting socket");
    exit(1);
  }

  char buf[BUFSIZ];
  int cnt;
  while ((cnt = read(client_socket, buf, BUFSIZ)) > 0) {
    for (int i = 0; i < cnt; i++) {
      buf[i] = toupper(buf[i]);
    }
    write(STDOUT_FILENO, buf, cnt);
  }

  if (cnt == -1) {
    close(client_socket);
    close(server_socket);
    unlink("./socket");
    perror("Error reading from socket");
    exit(1);
  }

  write(STDOUT_FILENO, "\n", 1);

  if (close(client_socket) == -1) {
    perror("Error close client_socket");
    exit(1);
  }

  if (close(server_socket) == -1) {
    perror("Error closing server_socket");
    exit(1);
  }

  if (unlink("./socket") == -1) {
    perror("Error unlinking socket filename");
    exit(1);
  }

  return 0;
}