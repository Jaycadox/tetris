#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "texture_store.h"
#define TETROMINO_DIM 4
#define TETROMINO_SIZE 5
#define TETROMINO_PIXEL_SIZE (8 * TETROMINO_SIZE)

struct tetromino_shape {
	uint16_t shapes[4];
	uint8_t num_shapes;
	bool filled;
	bool secondary;
	int8_t x_spawn;
	int8_t y_spawn;
};

struct tetromino {
	struct tetromino_shape* shape;
	uint8_t shape_idx;
	int8_t x;
	int8_t y;
};

void tetromino_draw(struct tetromino* tet, texture_store* store, float opacity);
void tetromino_cycle(struct tetromino* tet, uint8_t direction);
struct board;
bool tetromino_collision_test(struct tetromino* tet, struct board* b, int8_t x_off, int8_t y_off, int8_t shape_off);

static struct tetromino_shape STRAIGHT_SHAPE = {
	.shapes = {
		0b0000 +
		(0b0000 << 4) +
		(0b1111 << 8) +
		(0b0000 << 12),

		0b0010 +
		(0b0010 << 4) +
		(0b0010 << 8) +
		(0b0010 << 12),
	}, 
	.num_shapes = 2,
	.filled = true,
	.secondary = false,
	.x_spawn = -1,
	.y_spawn = -2,
};

static struct tetromino_shape T_SHAPE = {
	.shapes = {
		 0b0000 +
		(0b1110 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),

		 0b0100 +
		(0b1100 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),

		 0b0100 +
		(0b1110 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),

		 0b0100 +
		(0b0110 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 4,
	.filled = true,
	.secondary = false,
	.x_spawn = -2,
	.y_spawn = -1,
};

static struct tetromino_shape S_SHAPE = {
	.shapes = {
		 0b1100 +
		(0b0110 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),

		 0b0100 +
		(0b1100 << 4) +
		(0b1000 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 2,
	.filled = false,
	.secondary = true,
	.x_spawn = -2,
	.y_spawn = 0,
};

static struct tetromino_shape Z_SHAPE = {
	.shapes = {
		 0b0110 +
		(0b1100 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),

		 0b1000 +
		(0b1100 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 2,
	.filled = false,
	.secondary = false,
	.x_spawn = -2,
	.y_spawn = 0,
};

static struct tetromino_shape L_SHAPE = {
	.shapes = {
		 0b0000 +
		(0b1110 << 4) +
		(0b1000 << 8) +
		(0b0000 << 12),

		 0b0100 +
		(0b0100 << 4) +
		(0b0110 << 8) +
		(0b0000 << 12),

		 0b0010 +
		(0b1110 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),
		
		 0b1100 +
		(0b0100 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 4,
	.filled = false,
	.secondary = false,
	.x_spawn = -2,
	.y_spawn = -1,
};

static struct tetromino_shape J_SHAPE = {
	.shapes = {
		 0b0000 +
		(0b1110 << 4) +
		(0b0010 << 8) +
		(0b0000 << 12),

		 0b0110 +
		(0b0100 << 4) +
		(0b0100 << 8) +
		(0b0000 << 12),

		 0b1000 +
		(0b1110 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),
		
		 0b0100 +
		(0b0100 << 4) +
		(0b1100 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 4,
	.filled = false,
	.secondary = true,
	.x_spawn = -2,
	.y_spawn = -1,
};

static struct tetromino_shape SQUARE_SHAPE = {
	.shapes = {
		 0b1100 +
		(0b1100 << 4) +
		(0b0000 << 8) +
		(0b0000 << 12),
	}, 
	.num_shapes = 1,
	.filled = true,
	.secondary = false,
	.x_spawn = -2,
	.y_spawn = 0,
};

