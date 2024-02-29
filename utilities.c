// Constants source: https://en.wikipedia.org/wiki/Linear_congruential_generator
#define M 2147483648UL // Modulo (2^32)
#define A 1103515245 // Multiplier
#define C 12345 // Increment

// Create a variable for the seed.
unsigned static long random_seed = 1;

// Create a function to set the seed.
void set_random_seed(unsigned long seed) {
	random_seed = seed;
}

// Create a function to get the seed.
unsigned long get_random_seed() {
	// Generate the next random number.
	random_seed = (A * random_seed + C) % M;

	// Return byte 0-30 of the random number.
	return (random_seed & 0x7fffffff);
}

// Create a function to get a random integer.
int get_random_int(int min, int max) {
	// Calculate and return the random integer.
	return (get_random_seed() % (max - min + 1)) + min;
}

// Create a function to wait for a number of cycles.
void tick_delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}