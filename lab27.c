#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char input[BUFSIZ];
    FILE *fin, *fout;
    
    if ((fin = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        return 1;
    }
    if ((fout = popen("wc -l", "w")) == NULL) {
        perror("popen");
        return 1;
    }

    while (fgets(input, BUFSIZ, fin) != NULL) {
        if (strlen(input) == 1 && input[0] == '\n') {
            fputs(input, fout);
        }
    }
    fclose(fin);
    pclose(fout);
    return 0;
}

