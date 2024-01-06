#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 512

int main(int argc, char* argv[]){

	if(argc < 2)
	{
		fprintf(stderr, "No file\n");
		return 1;
	}

	int f_d;

	if ((f_d = open(argv[1], O_RDONLY))==-1){
		perror("Can`t open the file");
		return 1;
	}

	FILE *pipe = popen("./toUpper", "w");
	if (pipe == NULL){
		perror("failed to create pipe");
		return 1;
	}

	char outputBuf[SIZE];
        int readCount;
        while((readCount = read(f_d, outputBuf, SIZE)) != 0)
        {
        	fwrite(outputBuf,sizeof(char),readCount,pipe);     
        }
	close(f_d);
	pclose(pipe);
	return 0;
}
