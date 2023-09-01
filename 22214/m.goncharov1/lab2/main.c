#include<stdio.h>
#include<time.h> // for time function, man 3head
#include<stdlib.h>

int main() {

	time_t now; // time_t - type for storing seconds
	struct tm *sp; // struct tm declared in time.h
	(void) time( &now );
	
	
	if(putenv("TZ=America/Los_Angeles") != 0) {
		perror("putenv function error");
		exit(0);
	}

	sp = localtime(&now);
	if (sp == NULL) {
		perror("localtime function error");
		exit(0);
	}

	printf("%d/%d/%02d %d:%02d:%02d %s\n", 
			sp->tm_mon + 1, 
			sp->tm_mday, 
			sp->tm_year + 1900, 
			sp->tm_hour, 
			sp->tm_min,
		        sp->tm_sec,	
			tzname[sp->tm_isdst]
	);


	exit(0);
}
