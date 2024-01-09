#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

int main()
{
    char pattern[NAME_MAX  + 1];

    printf("Enter pattern: ");
    scanf("%s", pattern);

    if (strchr(pattern, '/'))
    {
        fprintf(stderr, "Slash in pattern\n");
        return -1;
    }

    char cwd[PATH_MAX + 1];

    if (!getcwd(cwd, sizeof(cwd)))
    {
	perror("Getcwd error");
	return -1;
    }

    DIR* dir;
    struct dirent* entry;
    int counter = 0;

    if (!(dir = opendir(cwd)))
    {
        perror("Opendir error");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (fnmatch(pattern, entry->d_name, FNM_PATHNAME) == 0)
        {
            printf("%s\n", entry->d_name);
	    ++counter;
        }
    }

    if (errno != 0)
    {
	perror("Readdir error");
	closedir(dir);
	return -1;
    }

    if (counter == 0)
    {
	printf("No such files by pattern: %s\n", pattern);
    }

    closedir(dir);
    return 0;
}
