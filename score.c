#include "score.h"

struct score score_new(void) {
	return (struct score) { .file = NULL, .taint = false };
}

uint32_t score_read(struct score* s) {
	FILE* file = fopen("tetris_highscore", "r");
	if (!file) {
		return 0;
	}

	uint32_t score = 0;
	if(!fread(&score, sizeof(score), 1, file)) {
		fclose(file);
		return 0;
	}
	fclose(file);

	return score;
}

uint32_t score_write(struct score* s, uint32_t score) {
	if (score <= score_read(s) || s->taint) {
		return score_read(s);
	}

	FILE* file = fopen("tetris_highscore", "w");
	if (!file) {
		return 0;
	}

	if (!fwrite(&score, sizeof(score), 1, file)) {
		fclose(file);
		return 0;
	}

	fclose(file);

	return score;
}
