#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char line[BUFSIZ];

    if(argc < 1){
        fprintf(stderr, "No arguments given");
    }

    if ((in = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    out = popen("wc -l", "w");
    if (out == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, BUFSIZ, in) != NULL)
        if (line[0] == '\n')
            fputs(line, out);

    fclose(in);
    
    if (pclose(out) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
