#pragma once
#include <stdio.h>
#include <stdint.h>
#include "tetromino.h"
#include "texture_store.h"

#define GAME_WIDTH 10
#define GAME_HEIGHT 20
#define MAX_SCORE_EMITTERS 5

static uint32_t COLOUR_PALLETS[][2] = {
	{0xFF521A, 0x5500BD},
	{0xFAF054, 0x00FEFC},
	{0x61170A, 0xCCFF98},
	{0x9E0085, 0xFFDC63},
	{0x6FFFB8, 0x005C8F},
};

struct score_emitter {
	uint16_t time_to_kill;
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
};

struct tetronimo_shape;
struct board {
	uint16_t tiles[GAME_HEIGHT];
	uint16_t filled[GAME_HEIGHT];
	uint16_t colour[GAME_HEIGHT];
	uint32_t score;
	uint32_t level;
	uint32_t lines;
	struct tetronimo_shape* next_shape;
	struct score_emitter emitters[MAX_SCORE_EMITTERS];
};

struct tetromino;
void board_engrave_tetromino(struct board* b, struct tetromino* tet);
void board_draw(struct board* b, texture_store* store);
void board_reset_tetromino(struct board* b, struct tetromino* t);
double board_get_fall_speed(struct board* b);
uint32_t board_get_needed_clear_lines(struct board* b);
void board_add_emitter(struct board* b, struct score_emitter* emitter);
void board_emit_line(struct board* b, uint8_t line_idx);
void board_emit_sonic_drop(struct board* b, struct tetromino* tet, uint8_t amount_fallen);
void board_draw_emitters(struct board* b);
void board_award_lines(struct board* b, uint8_t cleared_lines);
