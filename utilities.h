#ifndef UTILITIES_H
#define UTILITIES_H

void set_random_seed(unsigned long seed);
unsigned long get_random_seed();
int get_random_int(int min, int max);
void tick_delay(int);
void enable_interrupt(void);

#endif