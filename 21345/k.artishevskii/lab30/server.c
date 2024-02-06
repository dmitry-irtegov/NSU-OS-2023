#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUF_SIZE 256

int main(){
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s == -1){
		perror("Error create socket");
		return -1;
	}
	struct sockaddr_un address;
	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	char* path = "socket";
	strcpy(address.sun_path, path);
	if(bind(s, (struct sockaddr *) &address, sizeof(address)) == -1){
		perror("Error bind");
		unlink(path);
		return -1;
	}
	int backlog = 1;
	if(listen(s, backlog) == -1){
		perror("Error listen");
		unlink(path);
		return -1;
	}
	struct sockaddr_un client_address;
	socklen_t client_address_len = sizeof(client_address);
	int client_fd = accept(s, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
	if(client_fd < 0){
		perror("Error accept");
		unlink(path);
		return -1;
	}
	int size = 0;
	char string[BUF_SIZE];
	while((size = read(client_fd, string, BUF_SIZE)) > 0){
		for(size_t i = 0; i < size; ++i){
			putchar(toupper(string[i]));
		}
	}
	if(size == -1){
		perror("Error read");
		unlink(path);
		return -1;
	}
	unlink(path);
	return 0;
}
