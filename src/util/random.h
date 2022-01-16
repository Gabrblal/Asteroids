#ifndef RANDOM_H
#define RANDOM_H

/**
 * @brief Initialise thread safe random number generation.
 * 
 * Should be called on application start.
 */
void random_initialise();

/**
 * @brief Seed the random number generator. Thread safe.
 * 
 * @param seed The seed to use for the random number generator.
 */
void random_seed();

/**
 * @brief Get a random integer on bound 
 * 
 * random_seed() should be called on each thread before random numbers are
 * generated.
 * 
 * @returns A random number on range [min, max]
 */
int random_int(int min, int max);

/**
 * @brief Get a 
 */
double random_double(double min, double max);

/**
 * @brief Deinitialise thread safe random number generation.
 * 
 * Should be called on application end.
 */
void random_deinitialise();

#endif // RANDOM_H
