#pragma once 
#include "pallet_texture.h"
#include <stdio.h>
#include <stdint.h>

// A texture store is used to contain multiple pallet_textures and to change multiple pallets at once
typedef struct {
	pallet_texture tile; // Primary unfilled texture
	pallet_texture tile_filled; // Primary filled texture
	pallet_texture sec_tile; // Seconary unfilled texture
	pallet_texture sec_tile_filled; // Secondary filled texture
} texture_store;

// Creates a default texture_store (with default game textures) and load index 0 in COLOUR_PALLET (board.c)
texture_store texture_store_default(void);

// Creates texture store given custom images
texture_store texture_store_new(Image tile_img, Image filled_tile_img);

// Apply a pallet given primary and secondary colours to the corresponding pallet_texture items
void texture_store_apply_pallet(texture_store* store, uint32_t colour, uint32_t sec_colour);

// Get specific pallet_texture depending on whether the texture you want is filled and/or secondary
// Cannot return NULL
pallet_texture* texture_store_get_texture(texture_store* store, bool filled, bool secondary);

void texture_store_free(texture_store* store);
