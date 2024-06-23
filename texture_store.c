#include "texture_store.h"

/* --- Load raw bytes of texture BMPs --- */

static unsigned char tetris_base[]  = {
#include "tetris_base.bmp.h"
};


static unsigned char tetris_filled_base[]  = {
#include "tetris_filled_base.bmp.h"
};

#define ARRAYSIZE(x) sizeof(x) / sizeof(x[0])

texture_store texture_store_default() {
	Image img = LoadImageFromMemory(".bmp", tetris_base, ARRAYSIZE(tetris_base));
	Image filled_img = LoadImageFromMemory(".bmp", tetris_filled_base, ARRAYSIZE(tetris_filled_base));
	texture_store ts = texture_store_new(img, filled_img);
	texture_store_apply_pallet(&ts, 0xFF521A, 0x5500BD);
	return ts;
}

texture_store texture_store_new(Image tile_img, Image filled_tile_img) {
	texture_store ts = {
		.tile = pallet_texture_new(tile_img),
		.tile_filled = pallet_texture_new(filled_tile_img),
		.sec_tile = pallet_texture_new(tile_img),
		.sec_tile_filled = pallet_texture_new(filled_tile_img),
	};

	return ts;
}

void texture_store_apply_pallet(texture_store* store, uint32_t colour, uint32_t sec_colour) {
	pallet_texture_apply_pallet(&store->tile, colour);
	pallet_texture_apply_pallet(&store->tile_filled, colour);
	pallet_texture_apply_pallet(&store->sec_tile, sec_colour);
	pallet_texture_apply_pallet(&store->sec_tile_filled, sec_colour);
}

pallet_texture* texture_store_get_texture(texture_store* store, bool filled, bool secondary) {
	if (filled && secondary) {
		return &store->sec_tile_filled;
	} else if (filled && !secondary) {
		return &store->tile_filled;
	} else if (!filled && secondary) {
		return &store->sec_tile;
	} else {
		return &store->tile;
	}
}

void texture_store_free(texture_store* store) {
	pallet_texture_free(&store->tile);
	pallet_texture_free(&store->tile_filled);

	// HACK: image is already freed in the above lines, so only the texture is manually freed here
	UnloadTexture(store->sec_tile.texture);
	UnloadTexture(store->sec_tile_filled.texture);
}
