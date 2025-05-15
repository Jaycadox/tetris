#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tetromino.h"
#include "texture_store.h"

#define GAME_WIDTH 10
#define GAME_HEIGHT 20
#define MAX_SCORE_EMITTERS 5

// Games (wrapping) colour pallet depending on level
// format: {primary, secondary}, in 8bit rgb
static uint32_t COLOUR_PALLETS[][2] = {
	{0xFF521A, 0x5500BD},
	{0xFAF054, 0x00FEFC},
	{0x61170A, 0xCCFF98},
	{0x9E0085, 0xFFDC63},
	{0x6FFFB8, 0x005C8F},
};

// Structure representing line streak effects when player does action to gain score
struct score_emitter {
	uint16_t time_to_kill; // Used for opacity
			       // The rest are used for the position/scale of the rectangle draw
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
};

struct tetromino_shape;
struct board {
	// Whether tiles are present, filled (texture), or coloured (texture) are represented in an array of 16 bit numbers
	// Each number represents a specific row (rows are 10 long)
	// Each bit represents true or false as to whether or not that specific array is true in that location
	uint16_t tiles[GAME_HEIGHT];
	uint16_t filled[GAME_HEIGHT];
	uint16_t colour[GAME_HEIGHT];

	uint32_t score;
	uint32_t level;
	uint32_t lines;
	struct tetromino_shape* next_shape;
	struct score_emitter emitters[MAX_SCORE_EMITTERS];
};

struct tetromino;
// Writes tetromino to board "permanently" (fills tiles, filled, and colour fields)
void board_engrave_tetromino(struct board* b, struct tetromino* tet);

// Draw theengraved pieces on the board
void board_draw(struct board* b, texture_store* store);

// Resets (and rolls new next_shape) tetromino while respecting shape offsets
// Further, it checks if the initial spawn of the tetromino is colliding, if so, it resets the game
void board_reset_tetromino(struct board* b, struct tetromino* t, bool force_reset);

// RETURNS: number of seconds between fall movements
// Depends on the level
double board_get_fall_speed(struct board* b);

// RETURNS: number of lines you need to clear to advance the current level
uint32_t board_get_needed_clear_lines(struct board* b);

// Finds either a dead (time_to_kill = 0) or the closest to dead emitter and replaces it with `emitter`
void board_add_emitter(struct board* b, struct score_emitter* emitter);

// Creates an emitter that spans an entire row
void board_emit_line(struct board* b, uint8_t line_idx);

// Creates an emitter that spans the collumns of the tetromino
void board_emit_sonic_drop(struct board* b, struct tetromino* tet, uint8_t amount_fallen);

// Renders all current emitters (and ticks them so their time to live decreases)
void board_draw_emitters(struct board* b);

// Logic for increasing player score depending on how many lines they cleared
void board_award_lines(struct board* b, uint8_t cleared_lines);
