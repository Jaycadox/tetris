#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct score {
	FILE* file;
	bool taint;
};

struct score score_new(void);
uint32_t score_read(struct score* s);
uint32_t score_write(struct score* s, uint32_t score);

