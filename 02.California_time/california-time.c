#include <stdlib.h> 
#include <time.h>
#include <stdio.h>

void main()
{
    putenv("TZ=:America/Los_Angeles"); // LA timezone mathes for Californa.
    tzset();                            

    time_t lt;
    lt = time(NULL);
    struct tm *ptr;    
    ptr = localtime(&lt);
    
    printf("California local time is: %s", asctime(ptr));
}