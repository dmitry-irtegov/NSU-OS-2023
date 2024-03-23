#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>

static void prntmode(struct stat *stbuf, char * isFile) {
    if (S_ISDIR(stbuf->st_mode)) {
        printf("d");
    } else if (S_ISREG(stbuf->st_mode)) {
        printf("-");
        *isFile = 1;
    } else {
        printf("?");
    }
    printf("%c",(stbuf->st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c",(stbuf->st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c",(stbuf->st_mode & S_IXUSR) ? 'x' : '-');
    printf("%c",(stbuf->st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c",(stbuf->st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c",(stbuf->st_mode & S_IXGRP) ? 'x' : '-');
    printf("%c",(stbuf->st_mode & S_IROTH) ? 'r' : '-');
    printf("%c",(stbuf->st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c",(stbuf->st_mode & S_IXOTH) ? 'x' : '-');
}

static void prntuser(struct stat *stbuf) {
    struct passwd *pw;
    struct group *grp;
    pw = getpwuid(stbuf->st_uid);
    if (pw == NULL) {
	printf("%10d", stbuf->st_uid);
    } else {
        printf("%10s", pw->pw_name);
    }
    grp = getgrgid(stbuf->st_gid);
    if (grp == NULL) {
        printf("%10d", stbuf->st_gid);
    } else {
        printf("%10s", grp->gr_name);
    }
}

static void prntimes(struct stat *stbuf) {
    printf("%30s", ctime(&stbuf->st_mtime));
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Not enough args");
        return -1;
    }
    for (int i = 1; i < argc; i++) {
        struct stat stbuf;
        char isFile = 0;
        if (lstat(argv[i], &stbuf) == -1) {
            perror(argv[i]);
            continue;
        }
        prntmode(&stbuf, &isFile);
        printf("%5u\t", stbuf.st_nlink);
        prntuser(&stbuf);
        if (isFile) {
            printf("%12lli", (long long int)stbuf.st_size);
        } else {
            printf("       ");
        }
        printf("%20s", basename(argv[i]));
        prntimes(&stbuf);
    }

    return 0;
}
