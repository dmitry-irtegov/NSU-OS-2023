#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "argument amount error");
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");

    if (!fin)
    {
        perror("fopen error");
        return 1;
    }

    FILE* fpout = popen("tr '[:lower:]' '[:upper:]'", "w");

    if (fpout == NULL)
    {
        perror("popen error");
        if (fclose(fin) == -1)
        {
            perror("fclose error");
        }
        return 1;
    }

    size_t count = 0;
    char buff[BUFSIZ];

    while ((count = fread(buff, sizeof(char), BUFSIZ, fin)) != 0)
    {
        if (ferror(fin))
        {
            fprintf(stderr, "fread error");
            if (fclose(fin) == -1)
            {
                perror("input file fclose error");
            }
            if (pclose(fpout) == -1)
            {
                perror("output pipe fclose error");
            }
            return 1;
        }

        fwrite(buff, sizeof(char), count, fpout);

        if (ferror(fpout))
        {
            fprintf(stderr, "fwrite error");
            if (fclose(fin) == -1)
            {
                perror("fclose error");
            }
            if (pclose(fpout) == -1)
            {
                perror("pclose error");
            }
            return 1;
        }
    }

    if (pclose(fpout) == -1)
    {
        perror("pclose error");
        if (fclose(fin) == -1)
        {
            perror("fclose error");
        }
        return 1;
    }

    if (fclose(fin) == -1)
    {
        perror("fclose error");
        return 1;
    }

    return 0;
}
