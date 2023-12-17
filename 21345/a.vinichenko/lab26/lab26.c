#include <stdio.h>
#define TEXT_SIZE 15
int main()
{
    char line[TEXT_SIZE];

    FILE* fpin = popen("echo Hello, World", "r");
    FILE* fout = popen("tr [:lower:] [:upper:]", "w");

    if(!fpin)
    {
        perror("Error popen");
        return -1;
    }

    if(!fout)
    {
        perror("Error popen");
        return -1;
    }

    while(fgets(line, TEXT_SIZE, fpin) != NULL)
    {
        if(ferror(fpin))
        {
            perror("Error read");
            if(pclose(fpin))
            {
                perror("Error close");
            }
            return -1;
        }
        fputs(line, fout);
    }

    if(pclose(fpin) == - 1)
    {
        perror("Error close");
        return -1;
    }
    if(pclose(fout) == -1)
    {
        perror("Error close");
        return -1;
    }

}
