#include "tetromino.h"

void tetromino_draw(struct tetromino* tet, texture_store* store, float opacity) {
	// Fetch current shape (given rotation)
	const uint16_t shape = tet->shape->shapes[tet->shape_idx];

	// Loop through every index inside the shape
	for (uint8_t y_off = 0; y_off < TETROMINO_DIM; ++y_off) {
		// A shape is packed as a 64 bit number, or 4-16bit numbers right next to eachother
		// Unpack the specific 16-bit number given the row
		uint8_t row = (shape >> (y_off * 4)) & 0xff;

		for (uint8_t x_off = 0; x_off < TETROMINO_DIM; ++x_off) {
			// Shift to see if there exists a cell at this x position in this row
			if ((row >> x_off) & 1) {
				// If so, get the correct texture and draw it (while considering the tetromino position)
				Texture2D* tex = &texture_store_get_texture(store, tet->shape->filled, tet->shape->secondary)->texture;
				DrawTextureEx(*tex, (Vector2){(tet->x + x_off) * TETROMINO_PIXEL_SIZE, (tet->y + y_off) * TETROMINO_PIXEL_SIZE}, 0, TETROMINO_SIZE, (Color) { 255, 255, 255, 255.0 * opacity});
			}
		}
	}
}

void tetromino_cycle(struct tetromino* tet, uint8_t direction) {
	tet->shape_idx = (tet->shape_idx + direction) % tet->shape->num_shapes; // Modulo makes sure it doesn't overflow
}

bool tetromino_collision_test(struct tetromino* tet, struct board* b, int8_t x_off, int8_t y_off, int8_t shape_off) {
	// Modify members and get ready to return them to original state
	const uint8_t old_shape_idx = tet->shape_idx;
	tetromino_cycle(tet, shape_off);
	const uint16_t shape = tet->shape->shapes[tet->shape_idx];
	tet->shape_idx = old_shape_idx;

	const int8_t x = tet->x + x_off;
	const int8_t y = tet->y + y_off;
	
	// Same looping process as tetromino_draw func
	for (uint8_t y_off = 0; y_off < TETROMINO_DIM; ++y_off) {
		uint8_t row = (shape >> (y_off * 4)) & 0xff;
		for (uint8_t x_off = 0; x_off < TETROMINO_DIM; ++x_off) {
			if ((row >> x_off) & 1) {
				// If a tile is: too high, too low, or intersecting with engraved tiles, it is collided
				
				if (y + y_off >= GAME_HEIGHT) {
					return true;
				}

				if (x + x_off >= GAME_WIDTH || x + x_off < 0) {
					return true;
				}

				if (((b->tiles[y + y_off]) >> (x + x_off)) & 1) {
					return true;
				}

			}
		}
	}
	return false;
}
