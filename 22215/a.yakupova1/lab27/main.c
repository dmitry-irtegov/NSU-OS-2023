#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 2){
		perror("no file");
		return 1;
	}


	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		perror("error in opening file");
		return 1;
	}
	FILE *out = popen("wc -l", "w");
	if (out == NULL) {
		perror("error in popen");
		return 1;
	}

	char data[BUFSIZ];
	while (fgets(data, BUFSIZ, in) != NULL) {
		if (data[0] == '\n' && strlen(data) == 1){
			fputs(data, out);
		}
	}

	fclose(in);
	pclose(out);

	return 0;
}
