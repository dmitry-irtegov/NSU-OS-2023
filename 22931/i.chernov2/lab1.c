#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <ulimit.h>
#include <sys/resource.h>

extern char** environ;

int main(int argc, char *argv[])
 {
     char options[] = "cipsdvuU:C:V:";  /* valid options */
     int c;
     struct rlimit rlm;
     char cwd[PATH_MAX];

     printf("argc equals %d\n\n", argc);
     while ((c = getopt(argc, argv, options)) != EOF) {
      switch (c) {
      case 'd':
          if (getcwd(cwd, sizeof(cwd)) != NULL) {
              printf("Current working dir: %s\n\n", cwd);
          }
          else {
              perror("getcwd() error");
              return 1;
          }
          break;
      case 's':
          printf("Changing leader of the group\n\n");
          setpgid(0, getgid());
          break;
      case 'i':
          printf("Effective User ID = %d\n", geteuid());
          printf ("Real User ID = %d\n",getuid());
          printf("Effective Group ID = %d\n", getegid());
          printf ("Real Group ID = %d\n\n",getgid());
          break;
      case 'p':
          printf("Process ID = %d\n", getpid());
          printf("Parent Process ID = %d\n", getppid());
          printf("Group Process ID = %d\n\n", getpgrp());
          break;
      case 'c':
          getrlimit(RLIMIT_FSIZE, &rlm);
          printf("Current core file size limit = %ld\n", rlm.rlim_cur);
          printf("Maximum allowed = %4ld\n\n", rlm.rlim_max);
          break;
      case 'C':
          getrlimit(RLIMIT_FSIZE, &rlm);
          rlm.rlim_cur = atol(optarg);
          if(!setrlimit(RLIMIT_FSIZE, &rlm)){
               printf("Succsesfully changed to %ld\n\n", rlm.rlim_cur);
          }
          else{
               perror("strrlimit() error");
               return 1;
          }
          break;
      case 'u':
          printf("Size of ulimit is %ld\n\n", ulimit(UL_GETFSIZE));
          break;
      case 'U':
          if (ulimit(UL_SETFSIZE, atol(optarg)) == -1){
               perror("Wasn't able to change file limit\n");
               return 1;
          }
          printf("Size changed to %ld\n\n", atol(optarg));
          break;
      case 'v':
           printf("Current envs\n");
           for(int i = 0; environ[i]!=0; i++){
                printf("%d    %s\n", i, environ[i]);
           }
           printf("\n\n");
           break;
      case 'V':
           putenv(optarg);
           printf("Added new env %s\n\n", optarg);
           break;
      case '?':
          printf("invalid option is %c\n", optopt);
      }
     }
     return 0;
 }
