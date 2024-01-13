#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "No file.");
        return -1;
    }
    FILE* fin = fopen(argv[1], "r");
    if (fin != NULL) {
        perror("Open failed.");
        return -1;
    }
    FILE* fpout = popen("wc -l", "w");
    if (!fpout) {
        perror("Popen failed.");
        return -1;
    }
    char str[BUFSIZ];
    char last_element = '\n';
    while (fgets(str, BUFSIZ, fin)) {
        if (str[0] == '\n' && last_element == '\n') {
            fputs("\n", fpout);
        }
        last_element = str[strlen(str)-1];
    }
    fclose(fin);
    pclose(fpout);
    return 0;    
}
