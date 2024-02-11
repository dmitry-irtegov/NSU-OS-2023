#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++) {
        char *filename = argv[i];
        struct stat file_info;

        if (lstat(filename, &file_info) == -1) {
            perror(filename);
            continue;
        }

        mode_t mode = file_info.st_mode; 
        printf((S_ISDIR(mode)) ? "d" : ((S_ISREG(mode)) ? "-" : "?"));
        printf((mode & S_IRUSR) ? "r" : "-");
        printf((mode & S_IWUSR) ? "w" : "-");
        printf((mode & S_IXUSR) ? "x" : "-");
        printf((mode & S_IRGRP) ? "r" : "-");
        printf((mode & S_IWGRP) ? "w" : "-");
        printf((mode & S_IXGRP) ? "x" : "-");
        printf((mode & S_IROTH) ? "r" : "-");
        printf((mode & S_IWOTH) ? "w" : "-");
        printf((mode & S_IXOTH) ? "x" : "-");

        printf("  %-5u", file_info.st_nlink); 

        struct passwd *pw = getpwuid(file_info.st_uid);
        struct group *grp = getgrgid(file_info.st_gid);

        if (pw) {
            printf("  %-10s", pw->pw_name);
        } else {
            printf("  %-10u", file_info.st_uid);
        }

        if (grp) {
            printf("  %-10s", grp->gr_name);
        } else {
            printf("  %-10u", file_info.st_gid);
        }
        

        if (S_ISREG(mode)) { 
            printf("  %-10lld", (long long int)file_info.st_size);
        } else {
            printf("  %-10s", "");
        }

        printf("  %.24s", ctime(&file_info.st_mtime)); 

        printf("  %s\n", basename(filename)); 

    }


    exit(0);
}
