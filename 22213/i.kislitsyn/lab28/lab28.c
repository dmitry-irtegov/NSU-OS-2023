#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

int main()
{
    FILE *fptrs[2];
    srand((unsigned int)getpid());

    if (p2open("/bin/sort -n", fptrs) < 0)
    {
        perror("p2open error");
        exit(EXIT_FAILURE);
    }

    char str[4];
    for (int i = 0; i < 100; i++)
    {
        fprintf(fptrs[0], "%d\n", rand() % 100);
    }
    fclose(fptrs[0]);

    int index = 1;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (fgets(str, 4, fptrs[1]) == NULL)
            {
                fprintf(stderr, "fgets error\n");
                pclose(fptrs[1]);
                exit(EXIT_FAILURE);
            }

            if (index < 2 && str[index] != '\n')
            {
                index++;
            }
            str[index] = '\0';
            printf("%3s", str);
        }
        printf("\n");
    }
    pclose(fptrs[1]);

    exit(EXIT_SUCCESS);
}
