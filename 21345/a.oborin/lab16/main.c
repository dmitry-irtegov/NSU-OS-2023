#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main() {
	struct termios old_tty, new_tty;
	int stdin_descriptor = fileno(stdin);

	if (isatty(stdin_descriptor) == 0) {
		fprintf(stderr, "stdin not terminal\n");
		return 1;
	}
	tcgetattr(stdin_descriptor, &old_tty);

	new_tty = old_tty;
	new_tty.c_lflag = new_tty.c_lflag & ~(ISIG|ICANON|ECHO);
	new_tty.c_cc[VMIN] = 1;
	tcsetattr(stdin_descriptor, TCSANOW, &new_tty);
	setbuf(stdout, NULL);
	char input_char;

	while(1) {
		printf("How much is 3*3:");
		read(stdin_descriptor, &input_char, 1);
		printf("%c\n", input_char);
		if (input_char == '9') {
			printf("Excellent! It's right answer!\n");
			break;
		} else {
			printf("Wrong answer. Try again.\n\n");
		}
	}

	tcsetattr(stdin_descriptor, TCSAFLUSH, &old_tty);
	return 0;
}
