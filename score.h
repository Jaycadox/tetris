#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct score {
	FILE* file; // Unused
	bool taint; // Whether or not score should save
};

// Initialize score object
struct score score_new(void);

// Attempt to read the currently saved high-score
uint32_t score_read(struct score* s);

// Attempt to write a score to the high score
// NOTE: if the score you're trying to write is less than the saved one, the write will be cancelled
// RETURNS: the saved score or the inputted one, whichever is greater
uint32_t score_write(struct score* s, uint32_t score);

