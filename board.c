#include "board.h"
#include <string.h>

void board_engrave_tetromino(struct board* b, struct tetromino* tet) {
	const uint16_t shape = tet->shape->shapes[tet->shape_idx];
	uint8_t cleared_lines = 0;
	for (uint16_t y_off = 0; y_off < TETROMINO_DIM; ++y_off) {
		uint16_t row = (shape >> (y_off * 4)) & 0xff;
		for (uint16_t x_off = 0; x_off < TETROMINO_DIM; ++x_off) {
			if ((row >> x_off) & 1) { 
				b->tiles[tet->y + y_off] |= (1 << (tet->x + x_off));
				if (tet->shape->filled)
					b->filled[tet->y + y_off] |= (1 << (tet->x + x_off));
				if (tet->shape->secondary)
					b->colour[tet->y + y_off] |= (1 << (tet->x + x_off));

				// Line cleared
				if (b->tiles[tet->y + y_off] == 0x3FF) {
					memmove(b->tiles + 1, b->tiles, sizeof(b->tiles[0]) * (tet->y + y_off));
					memmove(b->colour + 1, b->colour, sizeof(b->colour[0]) * (tet->y + y_off));
					memmove(b->filled + 1, b->filled, sizeof(b->filled[0]) * (tet->y + y_off));
					b->tiles[0] = 0;
					b->colour[0] = 0;
					b->filled[0] = 0;
					++cleared_lines;
					board_emit_line(b, tet->y + y_off);
				}
			}
		}
	}
	board_award_lines(b, cleared_lines);
}

void board_draw(struct board* b, texture_store* store) {
	for (uint16_t y = 0; y < GAME_HEIGHT; ++y) {
		uint16_t row = b->tiles[y];
		for (uint16_t x = 0; x < GAME_WIDTH; ++x) {
			if ((row >> x) & 1) {
				Texture2D* tex = &texture_store_get_texture(store, b->filled[y] >> x & 1, b->colour[y] >> x & 1)->texture;
				DrawTextureEx(*tex, (Vector2){x * TETROMINO_PIXEL_SIZE, y * TETROMINO_PIXEL_SIZE}, 0, TETROMINO_SIZE, WHITE);
			}
		}
	}
	board_draw_emitters(b);
}

#define ARRAYSIZE(x) sizeof(x) / sizeof(x[0])

//#define STRAIGHT_ONLY
void board_reset_tetromino(struct board* b, struct tetromino* t) {
#ifdef STRAIGHT_ONLY
	struct tetromino_shape* shapes[] = { &STRAIGHT_SHAPE };
#else
	struct tetromino_shape* shapes[] = { &STRAIGHT_SHAPE, &T_SHAPE, &S_SHAPE, &Z_SHAPE, &L_SHAPE, &J_SHAPE, &SQUARE_SHAPE };
#endif
	if (b->next_shape) {
		t->shape = b->next_shape;
		b->next_shape = shapes[GetRandomValue(0, ARRAYSIZE(shapes) - 1)];
	} else {
		t->shape = shapes[GetRandomValue(0, ARRAYSIZE(shapes) - 1)];
		b->next_shape = shapes[GetRandomValue(0, ARRAYSIZE(shapes) - 1)];
	}
	t->shape_idx = 0;
	t->x = 5 + t->shape->x_spawn;
	t->y = 0 + t->shape->y_spawn;	


	if (tetromino_collision_test(t, b, 0, 0, 0)) {
		// Game lost
		*b = (struct board) {0};
		b->next_shape = shapes[GetRandomValue(0, ARRAYSIZE(shapes) - 1)];
	}
}

double board_get_fall_speed(struct board* b) {
	if (b->level > 8) {
		switch (b->level) {
			case 9: return 6.0 / 60.0;
			case 10:
			case 11:
			case 12: return 5.0 / 60.0;
			case 13:
			case 14:
			case 15: return 4.0 / 60.0;
			case 16:
			case 17:
			case 18: return 3.0 / 60.0;
			default: {
				 	if (b->level < 29) return 2.0 / 60.0;
					else return 1.0 / 60.0;
				 }
		}
	}
	return (48 - (5 * b->level)) / 60.0;
}

uint32_t board_get_needed_clear_lines(struct board* b) {
	return (b->level + 1) * 10;
}

void board_award_lines(struct board* b, uint8_t cleared_lines) {
	switch (cleared_lines) {
		case 1: b->score += 40 * (b->level + 1); break;
		case 2: b->score += 100 * (b->level + 1); break;
		case 3: b->score += 300 * (b->level + 1); break;
		case 4: b->score += 1200 * (b->level + 1); break;
	};

	b->lines += cleared_lines;
	if (b->lines >= board_get_needed_clear_lines(b)) {
		b->lines = 0;
		++(b->level);
	}
}

void board_add_emitter(struct board* b, struct score_emitter* emitter) {
	uint16_t record_num = 255;
	int8_t record_idx = -1;

	for (uint8_t i = 0; i < MAX_SCORE_EMITTERS; ++i) {
		if (b->emitters[i].time_to_kill == 0) {
			b->emitters[i] = *emitter;
			return;
		} else if (b->emitters[i].time_to_kill < record_num) {
			record_num = b->emitters[i].time_to_kill;
			record_idx = i;
		}
	}

	b->emitters[record_idx] = *emitter;
}

void board_emit_line(struct board* b, uint8_t line_idx) {
	struct score_emitter e = {
		.time_to_kill = 120,
		.x = 0,
		.y = line_idx,
		.width = GAME_WIDTH,
		.height = 1,	
	};
	board_add_emitter(b, &e);
}


void board_emit_sonic_drop(struct board* b, struct tetromino* tet, uint8_t amount_fallen) {
	uint16_t p1_x = 99;
	uint16_t p1_y = 99;

	uint16_t p2_x = 0;
	uint16_t p2_y = 0;

	const uint16_t shape = tet->shape->shapes[tet->shape_idx];
	for (uint16_t y_off = 0; y_off < TETROMINO_DIM; ++y_off) {
		uint16_t row = (shape >> (y_off * 4)) & 0xff;
		for (uint16_t x_off = 0; x_off < TETROMINO_DIM; ++x_off) {
			if ((row >> x_off) & 1) { 
				uint16_t x = tet->x + x_off;
				uint16_t y = tet->y + y_off;
				if (x > p2_x) p2_x = x;
				if (y > p2_y) p2_y = y;
			}
		}
	}

	tet->y += amount_fallen;
	for (uint16_t y_off = 0; y_off < TETROMINO_DIM; ++y_off) {
		uint16_t row = (shape >> (y_off * 4)) & 0xff;
		for (uint16_t x_off = 0; x_off < TETROMINO_DIM; ++x_off) {
			if ((row >> x_off) & 1) { 
				uint16_t x = tet->x + x_off;
				uint16_t y = tet->y + y_off;
				if (x < p1_x) p1_x = x;
				if (y < p1_y) p1_y = y;
			}
		}
	}
	tet->y -= amount_fallen;

	struct score_emitter e = {
		.time_to_kill = 20,
		.x = p1_x,
		.y = 0,
		.width = p2_x - p1_x + 1, 
		.height = p2_y - p1_y,	
	};
	board_add_emitter(b, &e);
}

void board_draw_emitters(struct board* b) {
	const uint16_t MAX_FRAMES_TILL_KILLED = 60;

	for(uint8_t i = 0; i < MAX_SCORE_EMITTERS; ++i) {
		struct score_emitter* e = &b->emitters[i];
		double opacity = (double)e->time_to_kill / (double)MAX_FRAMES_TILL_KILLED;
		DrawRectangle(e->x * TETROMINO_PIXEL_SIZE, e->y * TETROMINO_PIXEL_SIZE, e->width * TETROMINO_PIXEL_SIZE, e->height * TETROMINO_PIXEL_SIZE, (Color) { 255, 255, 255, opacity*30.0});
		if (e->time_to_kill > 0) {
			e->time_to_kill -= 1;
		}
	}

}
