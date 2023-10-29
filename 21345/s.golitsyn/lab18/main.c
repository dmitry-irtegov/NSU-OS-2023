#include <stdio.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>

char get_type(unsigned int st_mode) {
    if (S_ISDIR(st_mode)) {
        return 'd';
    }
    if (S_ISREG(st_mode)) {
        return '-';
    }
    return '?';
}

void get_permissions(unsigned int st_mode, char permissions[10]) {
    	permissions[0] = st_mode & S_IRUSR ? 'r' : '-';
	permissions[1] = st_mode & S_IWUSR ? 'w' : '-';
	permissions[2] = st_mode & S_IXUSR ? 'x' : '-';

	permissions[3] = st_mode & S_IRGRP ? 'r' : '-';
	permissions[4] = st_mode & S_IWGRP ? 'w' : '-';
	permissions[5] = st_mode & S_IXGRP ? 'x' : '-';

	permissions[6] = st_mode & S_IROTH ? 'r' : '-';
	permissions[7] = st_mode & S_IWOTH ? 'w' : '-';
	permissions[8] = st_mode & S_IXOTH ? 'x' : '-';

        permissions[9] = '\0';
}

int main(int argc, char *argv[]){
	for (int i = 1; i < argc; i++) {
		struct stat file_stats;

		if (lstat(argv[i], &file_stats) == -1) {
			perror("lstat syscall error");
			continue;
		}

		const char type = get_type(file_stats.st_mode);

		char permissions[10];
		get_permissions(file_stats.st_mode, permissions);

		const struct passwd *passwd_ptr = getpwuid(file_stats.st_uid);
		const struct group *group_ptr = getgrgid(file_stats.st_gid);

		char last_modification_time[26];
		if (ctime_r(&(file_stats.st_mtime), last_modification_time, sizeof(last_modification_time)) == NULL) {
			perror("last modification time error");
			continue;
		}
		last_modification_time[24] = '\0';

		printf("%c\t", type);
		printf("%s\t", permissions);
		printf("%u\t", file_stats.st_nlink);

		if (passwd_ptr) {
			printf("%s\t", passwd_ptr->pw_name);
		}
		else {
			printf("%u\t", file_stats.st_uid);
		}
		
		if (group_ptr) {
			printf("%s\t", group_ptr->gr_name);
		}
		else {
			printf("%u\t", file_stats.st_gid);
		}

		if (type == '-'){
			printf("%lu", file_stats.st_size);
		}
		printf("\t");

		printf("%s\t", last_modification_time);
		printf("%s\n", basename(argv[i]));
	}

	return 0;
}
