#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Not enough args\n");
	return -1;
    }

    FILE* fin = fopen(argv[1], "r");

    if (!fin)
    {
        perror("Can't open file");
        return -1;
    }

    FILE* pout = popen("wc -l", "w");

    if (!pout)
    {
        perror("Popen error");
        return -1;
    }

    char str[BUFSIZ];
    char last = '\n';

    while (fgets(str, BUFSIZ, fin))
    {
        if(ferror(fin))
        {
            perror("Read error");
            return -1;
        }
        if (str[0] == '\n' && last == '\n')
        {
            fputs("\n", pout);
        }
        last = str[strlen(str) - 1];
    }

    if(fclose(fin) == -1)
    {
        perror("Fclose error");
        return -1;
    }
    if(pclose(pout) == -1)
    {
        perror("Pclose error");
        return -1;
    }

    return 0;
}
