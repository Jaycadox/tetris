#include "score.h"

struct score score_new(void) {
	// TODO; file is currently unused
	return (struct score) { .file = NULL, .taint = false };
}

uint32_t score_read(struct score* s) {
	FILE* file = fopen("tetris_highscore", "r");
	if (!file) {
		return 0;
	}

	// Attempt to read score from file to variable
	uint32_t score = 0;
	if(!fread(&score, sizeof(score), 1, file)) {
		fclose(file);
		return 0;
	}
	fclose(file);

	return score;
}

uint32_t score_write(struct score* s, uint32_t score) {
	// Cancel write if requested score is less than the saved one
	// OR if the score is tainted (perhaps cheated)
	if (score <= score_read(s) || s->taint) {
		return score_read(s);
	}

	FILE* file = fopen("tetris_highscore", "w");
	if (!file) {
		return 0;
	}
	
	// Write score to disk
	if (!fwrite(&score, sizeof(score), 1, file)) {
		fclose(file);
		return 0;
	}

	fclose(file);

	return score;
}
