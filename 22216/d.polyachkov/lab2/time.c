#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

int main()
{
    time_t now;
    struct tm *sp;

    (void) time( &now );

    printf("%s", ctime( &now ) );

    setenv("TZ", "PST8PDT", 1); // Устанавливает значение переменной среды TZ, для того чтобы поменять часовой пояс
    tzset(); // Данная функция обновляет переменную среды TZ, чтобы программа работала с актуальным значением переменной среды

    sp = localtime(&now);
    printf("%d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year, sp->tm_hour,
        sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}

// struct tm {
//                int tm_sec;    /* секунды (0-60) */
//                int tm_min;    /* минуты (0-59) */
//                int tm_hour;   /* часы (0-23) */
//                int tm_mday;   /* день в месяце (1-31) */
//                int tm_mon;    /* месяц (0-11) */
//                int tm_year;   /* год - 1900 */
//                int tm_wday;   /* день недели (0-6, воскресенье = 0) */
//                int tm_yday;   /* день в году (0-365, 1 январь = 0) */
//                int tm_isdst;  /* летнее время */
//            };
