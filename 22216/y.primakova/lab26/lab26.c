#include <stdio.h>

int main() {
    char buff[BUFSIZ];
    FILE *fpin = popen("echo Hello, World!", "r");
    if (fpin == NULL) {
        perror("Popen failed.");
        return -1;
    }
    FILE *fpout = popen("tr [:lower:] [:upper:]", "w");
    if (fpout == NULL) {
        perror("Popen failed.");
        pclose(fpin);
        return -1;
    }
    while (fgets(buff, BUFSIZ, fpin) != NULL) {
        if (fputs(buff, fpout) == EOF) {
            perror("Fputs failed.");
            pclose(fpin);
            pclose(fpout);
            return -1;
        }
    }
    pclose(fpin);
    pclose(fpout);
    return 0;
}
