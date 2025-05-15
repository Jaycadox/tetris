#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "texture_store.h"
#define TETROMINO_DIM 4
#define TETROMINO_SIZE 5
#define TETROMINO_PIXEL_SIZE (8 * TETROMINO_SIZE)

// Defines properties intrinsic to a specific tetromino shape
struct tetromino_shape {
	uint16_t shapes[4]; // Rotation states
	uint8_t num_shapes;
	bool filled; // Should draw using filled texture
	bool secondary; // Should draw using secondary colour
	int8_t x_spawn; // X spawn offset
	int8_t y_spawn; // Y spawn offset
};

// Represents a specific tetromino on the screen
struct tetromino {
	struct tetromino_shape* shape; // Shape of tetromino
	uint8_t shape_idx; // Index into shape->shapes (rotation)
	int8_t x;
	int8_t y;
};

void tetromino_draw(struct tetromino* tet, texture_store* store, float opacity);

// Rotates (and wraps) tetromino in certain direction, specified by the sign (and magnitude if you're freaky) of direction.
void tetromino_cycle(struct tetromino* tet, uint8_t direction);

struct board;
// Tests to see if the tetromino collides when moved by x_off and y_off, and when shape_off rotations are applied
// RETURNS: whether or not a collision was detected
bool tetromino_collision_test(struct tetromino* tet, struct board* b, int8_t x_off, int8_t y_off, int8_t shape_off);

extern struct tetromino_shape STRAIGHT_SHAPE;
extern struct tetromino_shape T_SHAPE;
extern struct tetromino_shape SQUARE_SHAPE;
extern struct tetromino_shape L_SHAPE;
extern struct tetromino_shape J_SHAPE;
extern struct tetromino_shape Z_SHAPE;
extern struct tetromino_shape S_SHAPE;
