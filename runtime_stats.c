/*
 * Runtime Statistics Helper Functions
 * 
 * These functions are required when configGENERATE_RUN_TIME_STATS is enabled.
 * Implementation depends on the target platform.
 * 
 * For POSIX simulator, this uses clock_gettime().
 * For hardware targets, use a high-resolution timer.
 */

#include <time.h>
#include "FreeRTOS.h"

/* Runtime counter - increments on each tick */
static volatile unsigned long ulRunTimeCounter = 0;

/* Configure timer for runtime stats (called during initialization) */
void vConfigureTimerForRunTimeStats(void)
{
    /* For POSIX simulator, we use the system clock */
    /* The counter will be updated by the tick interrupt simulation */
    ulRunTimeCounter = 0;
}

/* Get the current runtime counter value */
unsigned long getRunTimeCounterValue(void)
{
    struct timespec ts;
    
    /* Use CLOCK_MONOTONIC for consistent timing */
    if(clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
    {
        /* Convert to microseconds for high resolution */
        return (unsigned long)(ts.tv_sec * 1000000UL + ts.tv_nsec / 1000UL);
    }
    
    /* Fallback to simple counter */
    return ulRunTimeCounter++;
}

