#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>


#define BUF_SIZE 100


int main(){
	FILE* pipe = popen("echo HeLlo, ANotHer proCess", "r");
	if(pipe == NULL){
		perror("Popen error");
		return -1;
	}
	char buf[BUF_SIZE];
	size_t read_count = fread(buf, 1, BUF_SIZE, pipe);
	if(ferror(pipe) != 0){
		perror("Fread error");
		return -1;
	}
	int status = pclose(pipe);
	if(status == -1){
		perror("Pclose error");
		return -1;
	}else{
		if(WIFEXITED(status)){
			printf("Child status code %d\n", WEXITSTATUS(status));
		}else if(WIFSIGNALED(status)){
			printf("Child signal code %d\n", WTERMSIG(status));
		}else if(WIFSTOPPED(status)){
			printf("Child signal code %d\n", WSTOPSIG(status));
		}
	}
	for(size_t i = 0; i < read_count; ++i){
		buf[i] = (char)toupper(buf[i]);
	}
	size_t written_count = fwrite(buf, read_count, 1, stdout);
	if(ferror(pipe) != 0){
		perror("Fwrite error");
		return -1;
	}
	return 0;
}
