#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char comand[strlen(argv[1] + 5)];
    strcpy(comand, "cat ");
    strcat(comand, argv[1]);

    FILE* input_pipe = popen(comand, "r");

    if(!input_pipe)
    {
        perror("Error open pipe");
        return -1;
    }

    size_t count = 0;
    char buff[BUFSIZ];

    while((count = fread(buff, sizeof(char), BUFSIZ, input_pipe)) != 0)
    {
        if(ferror(input_pipe))
        {
            perror("Error read from pipe");

            if(pclose(input_pipe) == -1)
            {
                perror("Error in closing pipe");
            }

            return -1;
        }

        for(size_t i = 0; i < count; ++i)
        {
            buff[i] = toupper(buff[i]);
	}

        fwrite(buff, sizeof(char), count, stdout);

	if(ferror(stdout))
        {
            perror("Error write to stdout");
            return -1;
        }

    }

    if(pclose(input_pipe) == -1)
    {
        perror("Error close pipe");
        return -1;
    }

    return 0;
}
