#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
	char type;
	char permissions[9];
	char *links;
	char *uid;
	char *gid;
	char *size;
	char last_modification_time[24];
	char *name;
} Output;

void get_type(char *type, struct stat *file_stats) {
	unsigned int st_mode = file_stats->st_mode;

	if (S_ISDIR(st_mode)) {
        *type = 'd';
		return;
    }

    if (S_ISREG(st_mode)) {
        *type = '-';
		return;
    }

	*type = '?';
} 

void get_permissions(char *permissions, struct stat *file_stats) {
	unsigned int st_mode = file_stats->st_mode;

    permissions[0] = st_mode & S_IRUSR ? 'r' : '-';
	permissions[1] = st_mode & S_IWUSR ? 'w' : '-';
	permissions[2] = st_mode & S_IXUSR ? 'x' : '-';

	permissions[3] = st_mode & S_IRGRP ? 'r' : '-';
	permissions[4] = st_mode & S_IWGRP ? 'w' : '-';
	permissions[5] = st_mode & S_IXGRP ? 'x' : '-';

	permissions[6] = st_mode & S_IROTH ? 'r' : '-';
	permissions[7] = st_mode & S_IWOTH ? 'w' : '-';
	permissions[8] = st_mode & S_IXOTH ? 'x' : '-';
}

char *get_links(struct stat *file_stats) {
	unsigned int links = file_stats->st_nlink;
	int length = snprintf(NULL, 0, "%u", links);
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%u", links);
	return str;
}

char *get_uid(struct stat *file_stats) {
	const struct passwd *passwd_ptr = getpwuid(file_stats->st_uid);

	if (passwd_ptr) {
		int length = snprintf(NULL, 0, "%s", passwd_ptr->pw_name);
		char *str = malloc(length + 1);
		snprintf(str, length + 1, "%s", passwd_ptr->pw_name);
		return str;
	}

	int length = snprintf(NULL, 0, "%u", file_stats->st_uid);
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%u", file_stats->st_uid);
	return str;
}

char *get_gid(struct stat *file_stats) {
	const struct group *group_ptr = getgrgid(file_stats->st_gid);

	if (group_ptr) {
		int length = snprintf(NULL, 0, "%s", group_ptr->gr_name);
		char *str = malloc(length + 1);
		snprintf(str, length + 1, "%s", group_ptr->gr_name);
		return str;
	}
	
	int length = snprintf(NULL, 0, "%u", file_stats->st_gid);
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%u", file_stats->st_gid);
	return str;
}

char *get_size(struct stat *file_stats) {
	int length = snprintf(NULL, 0, "%zu", file_stats->st_size);
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%zu", file_stats->st_size);
	return str;
}

void get_last_modification_time(char *last_modification_time, struct stat *file_stats) {
	char *time = ctime(&(file_stats->st_mtime));
	memcpy(last_modification_time, time, 24);
}

char *get_name(char *full_name) {
	int length = snprintf(NULL, 0, "%s", basename(full_name));
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%s", basename(full_name));
	return str;
}

Output create_output(struct stat *file_stats, char *name) {
	Output output;

	get_type(&(output.type), file_stats);
	get_permissions(output.permissions, file_stats);
	output.links = get_links(file_stats);
	output.uid = get_uid(file_stats);
	output.gid = get_gid(file_stats);
	output.size = get_size(file_stats);
	get_last_modification_time(output.last_modification_time, file_stats);
	output.name = get_name(name);

	return output;
}

void free_output(Output *output) {
	free(output->links);
	free(output->uid);
	free(output->gid);
	free(output->size);
	free(output->name);
}

int main(int argc, char *argv[]) {
	Output array[argc - 1];
	int in_use = 0;

	size_t max_links = 0;
	size_t max_uid = 0;
	size_t max_gid = 0;
	size_t max_size = 0;
	size_t max_name = 0;

	for (int i = 1; i < argc; i++) {
		struct stat file_stats;
		if (lstat(argv[i], &file_stats) == -1) {
			fprintf(stderr, "%s lstat syscall error\n");
			perror("");
			continue;
		}

		Output output = create_output(&file_stats, argv[i]);
		
		if (strlen(output.links) > max_links) {
			max_links = strlen(output.links);
		}
		if (strlen(output.uid) > max_uid) {
			max_uid = strlen(output.uid);
		}
		if (strlen(output.gid) > max_gid) {
			max_gid = strlen(output.gid);
		}
		if (strlen(output.size) > max_size) {
			max_size = strlen(output.size);
		}
		if (strlen(output.name) > max_name) {
			max_name = strlen(output.name);
		}

		array[in_use] = output;
		++in_use;
	}

	for (int i = 0; i < in_use; ++i) {
		Output current = array[i];
		printf("%c", current.type);
		printf("%.9s ", current.permissions);
		printf("%*s ", max_links, current.links);
		printf("%*s ", max_uid, current.uid);
		printf("%*s ", max_gid, current.gid);
		printf("%*s ", max_size, current.type == '-' ? current.size : "");
		printf("%.24s ", current.last_modification_time);
		printf("%*s\n", max_name, current.name);
		free_output(&current);
	}

	return 0;
}
