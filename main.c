#include <time.h>
#include <string.h>
#define SUPPORT_FILEFORMAT_BMP      1
#define SUPPORT_FILEFORMAT_TGA
#include <raylib.h>
#include <math.h>

#include "board.h"
#include "score.h"

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

int main (void) {
	// Perform some initial housework
	SetRandomSeed(time(NULL));
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(TETROMINO_PIXEL_SIZE * GAME_WIDTH + 300, TETROMINO_PIXEL_SIZE * GAME_HEIGHT, "Tetris");
	SetTargetFPS(60);

	/* --- General struct init --- */

	// Stores images/textures and can perform colour pallet swaps
	texture_store ts = texture_store_default();

	struct board b = {0};
	struct tetromino tet = {0};
	board_reset_tetromino(&b, &tet, false);

	struct tetromino preview = {0};
	struct tetromino ghost = {0};
	struct score s = score_new();

	/* --- Variables to keep track of state across frames --- */
	double last_tick = GetTime();
	uint32_t old_level = b.level;
	uint32_t old_score = b.score;
	float watermark_opacity = 1.0;
	uint32_t high_score = score_read(&s);

	char watermark_text[] = "made by jayphen (v0.2)              "; // Extra padding so change of game speed text has room
	double game_speed = 1.0;

	while (!WindowShouldClose()) {
		/* --- Update cross frame variables --- */
		double fall_speed = board_get_fall_speed(&b) / game_speed;
		double new_time = GetTime();


		// If we notice a change in our level
		if (old_level != b.level) {
			// Update the game pallet to correspond with the new level
			uint8_t pallet_idx = b.level % ARRAYSIZE(COLOUR_PALLETS);
			texture_store_apply_pallet(&ts, COLOUR_PALLETS[pallet_idx][0], COLOUR_PALLETS[pallet_idx][1]);	
		}
		old_level = b.level;

		// If we notice a change in our score
		if (old_score != b.score) {
			// Attempt to write it as a high-score
			high_score = score_write(&s, b.score);
		}
		old_score = b.score;

		/* --- Game logic and drawing  --- */

		BeginDrawing();
		ClearBackground((Color) { 12, 12, 12 });

		// Run code one every `fall_speed` seconds
		if (new_time - last_tick > fall_speed) {
			last_tick = new_time;
			
			// Test to see if the block would collide if it was shifted 1 down
			if (!tetromino_collision_test(&tet, &b, 0, 1, 0)) {
				++tet.y;
			} else {
				// If it would, engrave the current tetromino and spawn a new one
				board_engrave_tetromino(&b, &tet);
				board_reset_tetromino(&b, &tet, false);
			}
		}

		// Left and right movement with collision tests
		if ((IsKeyPressed(KEY_A) || IsKeyPressedRepeat(KEY_A)) && !tetromino_collision_test(&tet, &b, -1, 0, 0)) --tet.x;
		if ((IsKeyPressed(KEY_D) || IsKeyPressedRepeat(KEY_D)) && !tetromino_collision_test(&tet, &b, 1, 0, 0)) ++tet.x;

		// Copy the current tetromino and see how far it can be pushed down before it collides
		ghost = tet;
		for (int8_t new_y = ghost.y; new_y < GAME_HEIGHT; ++new_y) {
			if (tetromino_collision_test(&ghost, &b, 0, new_y - ghost.y, 0)) {
				// If we math h doublecollide here, it's safe to assume that the position above us is fine
				ghost.y = new_y - 1;
				break;
			}
		}

		// When the sonic drop is enabled and you're at a different y pos compared to the ghost
		if ((IsKeyPressed(KEY_S) || IsKeyPressedRepeat(KEY_S)) && ghost.y != tet.y) {
			// Add score proportional to how far away you were to the ghost
			b.score += ghost.y - tet.y;
			board_emit_sonic_drop(&b, &tet, ghost.y - tet.y);
			tet = ghost;

			// You are given a consistient amount of time after a sonic drop to re position
			last_tick = GetTime() - (board_get_fall_speed(&b) / (1.5 * game_speed));
		}

		// Rotational movement with collision tests
		if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) && !tetromino_collision_test(&tet, &b, 0, 0, -1)) tetromino_cycle(&tet, -1);
		if ((IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))  && !tetromino_collision_test(&tet, &b, 0, 0,  1)) tetromino_cycle(&tet,  1);

		// Cheat code
		if (IsKeyDown(KEY_EIGHT) && IsKeyDown(KEY_FOUR)) {
			board_award_lines(&b, 4);
			memset(b.tiles, 0, GAME_HEIGHT * sizeof(uint16_t));
			memset(b.colour, 0, GAME_HEIGHT * sizeof(uint16_t));
			memset(b.filled, 0, GAME_HEIGHT * sizeof(uint16_t));

			s.taint = true; // Makes it so the cheated score doesn't get saved as a high-score

			for (uint8_t i = GAME_HEIGHT - 4; i < GAME_HEIGHT; ++i) {
				board_emit_line(&b, i);
			}
		}

		// Process mouse wheel movements such that they are able to change the speed of the game
		int mouse_wheel_delta = GetMouseWheelMove();
		if (mouse_wheel_delta != 0) {
			double scaled_wheel_delta = (double)(mouse_wheel_delta) * 0.1;
			game_speed = fmin(fmax(1.0, game_speed + scaled_wheel_delta), 10.0);
			snprintf(watermark_text, sizeof(watermark_text), game_speed != 1.0 ? "Speed: %d%% (R to reset)" : "Speed: %d%%", (int)(game_speed * 100.0));
			watermark_opacity = 1.0;
		}

		if (IsKeyPressed(KEY_R)) {
			game_speed = 1.0;
			snprintf(watermark_text, sizeof(watermark_text), "Speed: %d%%", (int)(game_speed * 100.0));
			watermark_opacity = 1.0;
		}

		// Force reset game
		if (IsKeyPressed(KEY_C)) {
			board_reset_tetromino(&b, &tet, true);
		}

		// Draw own tetromino, ghost tetromino, and the board
		tetromino_draw(&tet, &ts, 1.0);
		tetromino_draw(&ghost, &ts, 0.4);
		board_draw(&b, &ts);
		
		/* --- Side panel drawing --- */
		DrawRectangle(TETROMINO_PIXEL_SIZE * GAME_WIDTH, 0, 300, TETROMINO_PIXEL_SIZE * GAME_HEIGHT, (Color) { 25, 25, 25, 255 });

		char score[16] = {0};
		snprintf(score, 16, "Score: %u", b.score);
		DrawText(score, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 10, 30, WHITE);

		char level[24] = {0};
		snprintf(level, 24, "Level: %u (%.1f%%)", b.level, ((double)b.lines / (double)board_get_needed_clear_lines(&b)) * 100.0);
		DrawText(level, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 50, 30, WHITE);

		// Draw 'Next tetromino' preview
		DrawText("Next", TETROMINO_PIXEL_SIZE * GAME_WIDTH + 110, 110, 30, WHITE);
		preview.shape = b.next_shape;
		preview.x = 13 + preview.shape->x_spawn;
		preview.y = 4 + preview.shape->y_spawn;
		tetromino_draw(&preview, &ts, 1.0);

		char highscore[28] = {0};
		snprintf(highscore, 28, "Highscore: %u", high_score);
		DrawText(highscore, TETROMINO_PIXEL_SIZE * GAME_WIDTH + 10, 300, 22, WHITE);

		// Spawn initial watermark that fades out eventually
		if (watermark_opacity > 0.01) {
			watermark_opacity -= 0.01;
			DrawText(watermark_text, 3, 3, 18, (Color) { 255, 255, 255, 255.0 * watermark_opacity });
		} else {
			watermark_opacity = 0;
		}

		EndDrawing();
	}
	texture_store_free(&ts);
}

