#include <termios.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	struct termios new_attr;
	struct termios old_attr;

	if(isatty(0) == 0){
		fprintf(stderr, "Fd 0 is no terminal\n");
		return -1;
	}

	if(tcgetattr(0, &old_attr) == -1){
		perror("Error tcgetattr");
		return -1;
	}
	
	new_attr = old_attr;
	new_attr.c_lflag &= ~(ICANON);
	new_attr.c_cc[VMIN] = 1;

	if(tcsetattr(0, TCSANOW, &new_attr) == -1){
		perror("Error update attr");
		return -1;
	}
	
	printf("How much is 2 * 2?\n");
	char c;
	read(0, &c, 1);
	if(c == '4'){
		printf("\nIt is right answer\n");
	}else{
		printf("\nIt is wrong answer\n");
	}

	if(tcsetattr(0, TCSANOW, &old_attr) == -1){
		perror("Error return old attr");
		return -1;
	}
	return 0;
}
