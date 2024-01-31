#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 256

int main(int argc, char* argv[]) {
	int f_d;
	if(argc < 2){
		f_d = STDIN_FILENO;
	}else if((f_d = open(argv[1], O_RDONLY)) == -1){
		perror("Error open");
		return -1;
	}
	struct sockaddr_un address;

	char *path = "socket";

	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s == -1) {
		perror("s() error");
		if(f_d != STDIN_FILENO){
			close(f_d);
		}
		return -1;
	}

	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, path);

	int addrlen = sizeof(address);
	if(connect(s, (struct sockaddr*)&address, (socklen_t)addrlen) == -1) {
		perror("connect() error");
		if(f_d != STDIN_FILENO){
			close(f_d);
		}
		return -1;
	}

	char buf[BUF_SIZE];
	int read_count = 0;
	while((read_count = read(f_d, buf, BUF_SIZE)) > 0){
		if(write(s, buf, read_count) != read_count){
			perror("Error write");
			if(f_d != STDIN_FILENO){
				close(f_d);
			}
			return -1;
		}
	}
	if(read_count == -1){
		perror("Error read");
		if(f_d != STDIN_FILENO){
			close(f_d);
		}
		return -1;
	}

	if(close(s) < 0){
		perror("close() error");
		if(f_d != STDIN_FILENO){
			close(f_d);
		}
		return -1;
	}
	if(f_d != STDIN_FILENO){
		close(f_d);
	}
	return 0;
}
