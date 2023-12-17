#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    FILE* file = fopen(argv[1], "r");

    if(!file)
    {
        perror("Error open file");
        return -1;
    }

    size_t count = 0;
    char buff[BUFSIZ];

    FILE* pipe = popen("tr '[:lower:]' '[:upper:]'", "w");

    if(!pipe)
    {
        perror("Error popen");
        if(fclose(file) == -1)
        {
            perror("Error in closing file");
        }
        return -1;
    }

    while((count = fread(buff, sizeof(char), BUFSIZ, file)) != 0)
    {
        if(ferror(file))
        {
            perror("Error read from file");
            if(fclose(file) == -1)
            {
                perror("Error in closing file");
            }
            if(pclose(pipe) == -1)
            {
                perror("Error close pipe");
            }
            return -1;
        }

        fwrite(buff, sizeof(char), count, pipe);

        if(ferror(pipe))
        {
            perror("Error write pipe");
            if(pclose(pipe))
            {
                perror("Error close pipe");
            }
            if(fclose(file))
            {
                perror("Error close file");
            }
            return -1;
        }
    }

    if(pclose(pipe) == -1)
    {
        perror("Error close pipe");
        return -1;
    }

    if(fclose(file) == -1)
    {
        perror("Error close file");
        return -1;
    }

    return 0;
}
