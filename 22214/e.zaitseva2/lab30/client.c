#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
  char* str_msg = "I am client";
  int socket_str, client_socket;

  socket_str = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_str == -1) {
    perror("Error creating socket");
    exit(1);
  }

  struct sockaddr_un cl_addr;
  memset(&cl_addr, 0, sizeof(cl_addr));
  cl_addr.sun_family = AF_UNIX;
  strcpy(cl_addr.sun_path, "./socket");
  client_socket = connect(socket_str, (struct sockaddr *)&cl_addr, sizeof(cl_addr));

  if (client_socket == -1) {
    close(socket_str);
    perror("Errror connecting");
    exit(1);
  }

  if (write(socket_str, str_msg, strlen(str_msg)) == -1) {
	perror("Can't write to server");
	exit(1);
}
  close(socket_str);
  return 0;
}