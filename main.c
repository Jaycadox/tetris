#include <time.h>
#include <string.h>
#define SUPPORT_FILEFORMAT_TGA
#include <raylib.h>

#include "board.h"
#include "score.h"

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
int main (void) {
	SetRandomSeed(time(NULL));
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(TETROMINO_PIXEL_SIZE * GAME_WIDTH + 300, TETROMINO_PIXEL_SIZE * GAME_HEIGHT, "Tetris");
	SetTargetFPS(60);
	texture_store ts = texture_store_default();

	struct board b = {0};
	struct tetromino tet = {0};
	struct tetromino preview = {0};
	struct tetromino ghost = {0};
	struct score s = score_new();

	board_reset_tetromino(&b, &tet);

	double last_tick = GetTime();

	uint32_t old_level = b.level;
	uint32_t old_score = b.score;

	float watermark_opacity = 1.0;
	uint32_t high_score = score_read(&s);

	while (!WindowShouldClose()) {
		double fall_speed = board_get_fall_speed(&b);
		BeginDrawing();
		ClearBackground((Color) { 12, 12, 12 });
		double new_time = GetTime();
		if (new_time - last_tick > fall_speed) {
			last_tick = new_time;
			if (!tetromino_collision_test(&tet, &b, 0, 1, 0)) {
				++tet.y;
			} else {
				board_engrave_tetromino(&b, &tet);
				board_reset_tetromino(&b, &tet);
			}
		}

		ghost = tet;
		for (int8_t new_y = ghost.y; new_y < GAME_HEIGHT; ++new_y) {
			if (tetromino_collision_test(&ghost, &b, 0, new_y - ghost.y, 0)) {
				ghost.y = new_y - 1;
				break;
			}
		}

		// HACK
		if (old_level != b.level) {
			uint8_t pallet_idx = b.level % ARRAYSIZE(COLOUR_PALLETS);
			texture_store_apply_pallet(&ts, COLOUR_PALLETS[pallet_idx][0], COLOUR_PALLETS[pallet_idx][1]);	
		}
		old_level = b.level;


		if (old_score != b.score) {
			high_score = score_write(&s, b.score);
		}
		old_score = b.score;

		if (IsKeyPressed(KEY_A) && !tetromino_collision_test(&tet, &b, -1, 0, 0)) --tet.x;
		if (IsKeyPressed(KEY_D)  && !tetromino_collision_test(&tet, &b, 1, 0, 0)) ++tet.x;
		if (IsKeyPressed(KEY_S) && ghost.y != tet.y) {
			b.score += ghost.y - tet.y;
			board_emit_sonic_drop(&b, &tet, ghost.y - tet.y);
			tet = ghost;

			// You are given a consistient amount of time after a sonic drop to re position
			last_tick = GetTime() - (board_get_fall_speed(&b) / 1.5);
		}

		if (IsKeyPressed(KEY_RIGHT) && !tetromino_collision_test(&tet, &b, 0, 0, -1)) tetromino_cycle(&tet, -1);
		if (IsKeyPressed(KEY_LEFT)  && !tetromino_collision_test(&tet, &b, 0, 0,  1)) tetromino_cycle(&tet,  1);

		// Cheat code
		if (IsKeyDown(KEY_EIGHT) && IsKeyDown(KEY_FOUR)) {
			board_award_lines(&b, 4);
			memset(b.tiles, 0, GAME_HEIGHT * sizeof(uint16_t));
			memset(b.colour, 0, GAME_HEIGHT * sizeof(uint16_t));
			memset(b.filled, 0, GAME_HEIGHT * sizeof(uint16_t));
			s.taint = true;

			for (uint8_t i = GAME_HEIGHT - 4; i < GAME_HEIGHT; ++i) {
				board_emit_line(&b, i);
			}
		}

		tetromino_draw(&tet, &ts, 1.0);
		tetromino_draw(&ghost, &ts, 0.1);
		board_draw(&b, &ts);
		DrawRectangle(TETROMINO_PIXEL_SIZE * GAME_WIDTH, 0, 300, TETROMINO_PIXEL_SIZE * GAME_HEIGHT, (Color) { 25, 25, 25, 255 });

		char score[16] = {0};
		snprintf(score, 16, "Score: %u", b.score);
		DrawText(score, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 10, 30, WHITE);

		char level[24] = {0};
		snprintf(level, 24, "Level: %u (%.1f%)", b.level, ((double)b.lines / (double)board_get_needed_clear_lines(&b)) * 100.0);
		DrawText(level, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 50, 30, WHITE);
		DrawText("Next", TETROMINO_PIXEL_SIZE * GAME_WIDTH + 110, 110, 30, WHITE);
		preview.shape = b.next_shape;
		preview.x = 13 + preview.shape->x_spawn;
		preview.y = 4 + preview.shape->y_spawn;
		tetromino_draw(&preview, &ts, 1.0);

		char highscore[28] = {0};
		snprintf(highscore, 28, "Highscore: %u", high_score);
		DrawText(highscore, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 300, 22, WHITE);

		if (watermark_opacity > 0.01) {
			watermark_opacity -= 0.01;
			DrawText("made by jayphen (v0.1)", 0, 0, 12, (Color) { 255, 255, 255, 255.0 * watermark_opacity });
		} else {
			watermark_opacity = 0;
		}

		EndDrawing();
	}
	texture_store_free(&ts);
}

