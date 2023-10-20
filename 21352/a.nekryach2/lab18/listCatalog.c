#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
#include <libgen.h>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>

int differenceOfMonths(struct tm* time, struct tm* currentTime)
{
    return currentTime->tm_mon + 11*(currentTime->tm_year - time->tm_year) - time->tm_mon;
}

void printTime(struct tm* timeFile,time_t* timeFileSeconds)
{
    char date[13];

    struct tm currentTime;
    time_t currentTimeSeconds = time(NULL);
    currentTime = *localtime(&currentTimeSeconds);
    double diffSeconds = difftime(currentTimeSeconds,*timeFileSeconds);
    int diffMon = differenceOfMonths(timeFile, &currentTime);
    
    if(diffMon > 6 || (diffSeconds/3600 < 0))
    {
	strftime(date,sizeof(date),"%b %d  %Y", timeFile);	
    }
    else
    {
	strftime(date, sizeof(date),"%b %d %H:%M", timeFile);
    }    
    printf("%s", date);
}

void printFtype(struct stat* s)
{
    if (S_ISDIR(s->st_mode))
    {
        printf("d");
    }
    else if (S_ISREG(s->st_mode))
    {
        printf("-");
    }
    else
    {
        printf("?");
    }
}

void printFpermissions(struct stat* s)
{
    printf("%c",(s->st_mode & S_IRUSR)?'r':'-');
    printf("%c",(s->st_mode & S_IWUSR)?'w':'-');
    printf("%c",(s->st_mode & S_IXUSR)?'x':'-');
    printf("%c",(s->st_mode & S_IRGRP)?'r':'-');
    printf("%c",(s->st_mode & S_IWGRP)?'w':'-');
    printf("%c",(s->st_mode & S_IXGRP)?'x':'-');
    printf("%c",(s->st_mode & S_IROTH)?'r':'-');
    printf("%c",(s->st_mode & S_IWOTH)?'w':'-');
    printf("%c ",(s->st_mode & S_IXOTH)?'x':'-');
}

void ls(char* name)
{
    struct stat s = {0};
    if (stat(name, &s))
    {
        perror("Unable to get stat");
        return;
    }

    struct group* gr = getgrgid(s.st_gid);
    struct passwd* pw = getpwuid(s.st_uid);
    bool isFile = S_ISREG(s.st_mode);
    struct tm timeFile = *localtime(&s.st_mtime);
   
    printFtype(&s);
    printFpermissions(&s);

    if (isFile)
    {
        printf("%20lli",(long long int)s.st_size);
    }
    else
    {
        printf("%20s","");
    }

    if (pw){
        printf("%20s ", pw->pw_name);
    }
    else{
	printf("%20u ", s.st_uid);
    }
    if (gr)
    {
        printf("%20s ", gr->gr_name);
    }
    else
    {
       printf("%20u ", s.st_gid);
    }
    printTime(&timeFile,&s.st_mtime); 

    printf(" %s\n", basename(name));

}

int main(int argc, char** args)
{
    if (argc < 2)
    {
        printf("Not enough args\n");
        return -1;
    }
    for (int i = 1; i < argc; ++i)
    {
        ls(args[i]);
    }

    return 0;
}
