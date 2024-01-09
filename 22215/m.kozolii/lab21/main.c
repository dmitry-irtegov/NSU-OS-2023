#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int n;
int is_cont = 1;

static void sig_hand(int signo){
	char str[255];
	if(signo == SIGQUIT){
		snprintf(str, 255, "Beeped %d\n", n);
		write(STDOUT_FILENO, str, strlen(str));
		is_cont = 0;
	}
	if(signo == SIGINT){
		if(write(STDOUT_FILENO, "\07", sizeof(char)) == -1){
			strerror_r(errno, str, 255);
			write(STDERR_FILENO, "Error in write ", 16);
			write(STDERR_FILENO, str, strlen(str));
			write(STDERR_FILENO, "\n", 1);
		} else {
			n++;
		}
	}
}

int main(){
	if(sigset(SIGINT, sig_hand) == SIG_ERR){
		perror("cannot set handler of SIGINT comm");
		return -1;
	}
	if(sigset(SIGQUIT, sig_hand) == SIG_ERR){
		perror("cannot set handler of SIGINT comm");
		return -1;
	}

	while(is_cont) {
		pause();
	}
	return 0;
}