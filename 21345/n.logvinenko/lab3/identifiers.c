#include <stdio.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

static void open_close_file(const char* const  path_to_file)
{
    FILE* file = fopen(path_to_file, "r");

    if (!file)
    {
        perror("Failure while opening file\n\n");
        return;
    }

    printf("File was opened successfully!\n\n");
    fclose(file);
}

int main(const int argc, const char** argv)
{
    if (argc < 2)
    {
        printf("Not enough arguments. Provide path to file that will be opened\n");
        return 1;
    }

    printf("real user id: %d\neffective user id: %d\n", getuid(), geteuid());
    open_close_file(argv[1]);

    if (seteuid(getuid()) != 0)
    {
	    perror("Something went wrong. Can not change uid.");
	    return errno;
    }

    printf("After seteuid():\nreal user id: %d\neffective user id: %d\n", getuid(), geteuid());
    open_close_file(argv[1]);

    return 0;
}
