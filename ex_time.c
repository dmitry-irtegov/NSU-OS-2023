#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
//    time_t t = time(NULL);
//    printf("UTC:       %s", asctime(gmtime(&t)));
//    printf("local:     %s", asctime(localtime(&t)));
//    // POSIX-specific
//    putenv("TZ=Asia/Japan");
//    printf("Singapore: %s", asctime(localtime(&t)));
    time_t curTime;
    struct tm *date;
    time(&curTime);             //put milliseconds time in var
    printf("UTC %s", asctime(gmtime(&curTime)));
    date = gmtime(&curTime);    //put time in comfort struct with fields
//    printf("minutes %d hours %d mday %d month %d year %d isSummer %d\n", date->tm_min, date->tm_hour, date->tm_mday, date->tm_mon, date->tm_year, date->tm_isdst);
//    printf("minutes %d hours %d mday %d month %d year %d  isSummer %d\n", date->tm_min, date->tm_hour, date->tm_mday, date->tm_mon, date->tm_year, date->tm_isdst);
    date->tm_hour -= 8;
    if((date->tm_mon < 11 && date->tm_mon > 3) || (date->tm_mon == 1 && date-> tm_mday < 15) || (date->tm_mon == 3 && date->tm_mday > 12)) date->tm_hour++;
    if (date->tm_hour < 0) {
        date->tm_hour += 24;
        date->tm_mday-= 1;
    }
//    printf("minutes %d hours %d mday %d month %d year %d  isSummer %d\n", date->tm_min, date->tm_hour, date->tm_mday, date->tm_mon, date->tm_year, date->tm_isdst);
    printf("Sun beautiful place date and time are %s", asctime(date));
//    printf("Sun beautiful place date and time are %s", asctime(gmtime(&curTime)));
    return 0;
}































































































//#include <unistd.h>
//
//#include <sys/resource.h>
//
//extern char **environ;
//
//void main(int argc, char *argv[]) {
//    char options[] = "uvfspidg:"; /* valid options */
//    int c, invalid = 0, dflg = 0, fflg = 0, gflg = 0;
//    char *f_ptr, *g_ptr;
//
//    printf("argc equals %d\n", argc);
//    while ((c = getopt(argc, argv, options)) != EOF) {
//        switch (c) {
////            case 'i':
////                printf("---i---\nuser id is %d\n", getuid());
////                printf("effective user id is %d\n", geteuid());
////                break;
////            case 's':
////                printf("---s---\nsetpgid %d", getpid());
////                break;
////            case 'p':
////                printf("---p---\nprocess id is %d\n", getpid());
////                printf("parents's process id is %d\n", getppid());
////                printf("group's process id is %d\n", getpgrp());
//            break;
//            case 'd':
//                printf("---d---\n");
//                char *path = (char *) calloc(1024, 1);
//                getcwd(path, 1024);
//                printf("current directory is %s\n", path);
//                break;
//            case 'f':
//                fflg++;
//                f_ptr = optarg;
//                break;
//            case 'v':
//                printf("---v---\n");
////                char *val;
////                val = getenv("HOME");
////                if(val) printf("val is %s\n", val);
////                else printf("error in getting val\n");
//                char **env = environ;
//                while (*env != NULL) {
//                    printf("%s\n---------------------------\n", *env);
//                    env++;
//                }
//                break;
//            case 'u':
//                printf("---u---\n");
//                struct rlimit limits;
//                getrlimit(RLIMIT_NOFILE, &limits);
//                printf("Soft limit for number of open files: %ld\n", limits.rlim_cur);
//                printf("Hard limit for number of open files: %ld\n", limits.rlim_max);
//                getrlimit(RLIMIT_AS, &limits);
//                printf("Soft limit for number of open files: %ld\n", limits.rlim_cur);
//                printf("Hard limit for number of open files: %ld\n", limits.rlim_max);
//                getrlimit(RLIMIT_CPU, &limits);
//                printf("Soft limit for number of open files: %ld\n", limits.rlim_cur);
//                printf("Hard limit for number of open files: %ld\n", limits.rlim_max);
//            case 'g':
//                gflg++;
//                g_ptr = optarg;
//                break;
//            case '?':
//                printf("invalid option is %c\n", optopt);
//                invalid++;
//        }
//    }
//    printf("dflg equals %d\n", dflg);
//    if (fflg)
//        printf("f_ptr points to %s\n", f_ptr);
//    if (gflg)
//        printf("g_ptr points to %s\n", g_ptr);
//    printf("invalid equals %d\n", invalid);
//    printf("optind equals %d\n", optind);
//    if (optind < argc) { printf("next parameter = %s\n", argv[optind]); }
//}
//
////осталось сделать
////V-i  Печатает реальные и эффективные идентификаторы пользователя и группы.
////?-s  Процесс становится лидером группы. Подсказка: смотри setpgid(2).
////V-p  Печатает идентификаторы процесса, процесса-родителя и группы процессов.
////?-u  Печатает значение ulimit
////?-Unew_ulimit  Изменяет значение ulimit. Подсказка: смотри atol(3C) на странице руководства strtol(3C)
////?-c  Печатает размер в байтах core-файла, который может быть создан.
////-Csize  Изменяет размер core-файла
////V-d  Печатает текущую рабочую директорию
////V-v  Распечатывает переменные среды и их значения
////-Vname=value  Вносит новую переменную в среду или изменяет значение существующей переменной.
