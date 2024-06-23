#pragma once
#include "pallet_texture.h"
#include <stdio.h>
#include <stdint.h>

typedef struct {
	pallet_texture tile;
	pallet_texture tile_filled;
	pallet_texture sec_tile;
	pallet_texture sec_tile_filled;
} texture_store;

texture_store texture_store_default(void);
texture_store texture_store_new(Image tile_img, Image filled_tile_img);
void texture_store_apply_pallet(texture_store* store, uint32_t colour, uint32_t sec_colour);
pallet_texture* texture_store_get_texture(texture_store* store, bool filled, bool secondary);
void texture_store_free(texture_store* store);
