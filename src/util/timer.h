#ifndef TIME_H
#define TIME_H

#include <stdint.h>

typedef uint64_t Time;

/**
 * Initialise the time interface.
 * 
 * Creates the SDL timer responsible for incrementing the global time, and
 * initialises 
 */
void time_initialise();

/**
 * Get the current global time since the program started.
 * 
 * @return The global time.
 */
Time time_global();

/**
 * Determine how many milliseconds have elapsed since the last time this time
 * was checked.
 * 
 * Sets the time to the current global time.
 * 
 * @param timer The timer to check.
 * @return How many milliseconds have elapsed since this timer was last checked.
 */
Time time_since_last(Time *timer);

/**
 * Elgantly destroy data related to the time interface. Using time after
 * this function has been called is undefined.
 */
void time_deinitialise();

#endif // TIME_H