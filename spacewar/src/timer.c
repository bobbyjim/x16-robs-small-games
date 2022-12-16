#include <time.h>
#include <stdlib.h>

#include "timer.h"

struct timespec ts;

void pause_jiffies(unsigned jiffies)
{
    clock_t now = clock();
    if (clock() < now + jiffies)
    {
       while(clock() < now + jiffies) { /* wait */ }
    }
}

long getMillis()
{
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_nsec / 1000000;
}
