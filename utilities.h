#ifndef UTILITIES_H
#define UTILITIES_H

// Constants source: https://en.wikipedia.org/wiki/Linear_congruential_generator
#define M 4294967296 // Modulo (2^32)
#define A 1664525 // Multiplier
#define C 1013904223 // Increment

void set_random_seed(unsigned long seed);
unsigned long get_random_seed();
int get_random_int(int min, int max);
void tick_delay(int);
void enable_interrupt(void);

#endif