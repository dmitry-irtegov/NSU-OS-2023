#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <libgen.h>
#include <string.h>

void getPermissionString(struct stat *fileStat, char *permissionString) {
    switch (fileStat->st_mode & S_IFMT) {
        case S_IFDIR:
            permissionString[0] = 'd';
            break;
        case S_IFREG:
            permissionString[0] = '-';
            break;
        case S_IFBLK:
            permissionString[0] = 'b';
            break;
        case S_IFCHR:
            permissionString[0] = 'c';
            break;
        case S_IFLNK:
            permissionString[0] = 'l';
            break;
        case S_IFIFO:
            permissionString[0] = 'p';
            break;
        case S_IFSOCK:
            permissionString[0] = 's';
    }
    permissionString[1] = (fileStat->st_mode & S_IRUSR) ? 'r' : '-';
    permissionString[2] = (fileStat->st_mode & S_IWUSR) ? 'w' : '-';
    permissionString[3] = (fileStat->st_mode & S_IXUSR) ? 'x' : '-';
    permissionString[4] = (fileStat->st_mode & S_IRGRP) ? 'r' : '-';
    permissionString[5] = (fileStat->st_mode & S_IWGRP) ? 'w' : '-';
    permissionString[6] = (fileStat->st_mode & S_IXGRP) ? 'x' : '-';
    permissionString[7] = (fileStat->st_mode & S_IROTH) ? 'r' : '-';
    permissionString[8] = (fileStat->st_mode & S_IWOTH) ? 'w' : '-';
    permissionString[9] = (fileStat->st_mode & S_IXOTH) ? 'x' : '-';
}

int main(int argc, char *argv[]) {
    struct stat fileStat;
    char permissionString[] = "-rwxrwxrwx";
    struct passwd *fileOwner;
    struct group *fileGroup;

    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &fileStat) == -1) {
            perror(argv[i]);
            continue;
        }

        getPermissionString(&fileStat, permissionString);
        fileOwner = getpwuid(fileStat.st_uid);
        fileGroup = getgrgid(fileStat.st_gid);

        printf("%s %3lu ", permissionString, fileStat.st_nlink);

        if (fileOwner == NULL) {
            printf("%d ", fileStat.st_uid);
        } else {
            printf("%s ", fileOwner->pw_name);
        }

        if (fileGroup == NULL) {
            printf("%d ", fileStat.st_gid);
        } else {
            printf("%s ", fileGroup->gr_name);
        }

        printf("%7ld %.24s %s\n", fileStat.st_size,
               ctime(&fileStat.st_mtime), basename(argv[i]));
    }

    return 0;
}
