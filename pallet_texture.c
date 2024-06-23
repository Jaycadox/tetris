#include "pallet_texture.h"
#include <assert.h>

#define BASE_IMG_R 0x60
#define BASE_IMG_G 0x70
#define BASE_IMG_B 0x80

typedef struct pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel;


pallet_texture pallet_texture_new(Image img) {
	// This is so we can assume that one pixel is three bytes when calculating size of image
	assert(img.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8);
	size_t img_sz = img.width * img.height * 3;
	
	// Find and create mask of pixels that have placeholder colours
	// This works because Tetris tiles are 8x8 so a 64-bit mask can be used
	size_t mask = 0;
	for (uint8_t i = 0; i < 64; ++i) {
		if (((pixel*)img.data)[i].r == BASE_IMG_R) {
			mask |= ((size_t)1 << i);
		}
	}

	pallet_texture ts = {
		.image = img,
		.size = img_sz,
		.mask = mask,
		.texture = LoadTextureFromImage(img),
	};


	return ts;
}


void pallet_texture_apply_pallet(pallet_texture* texture, uint32_t colour) {
	const uint8_t r = colour >> 16;
	const uint8_t g = (colour >> 8) & 0xff;
	const uint8_t b = colour & 0xff;
	pixel p = { .r = r, .g = g, .b = b};

	for (uint8_t j = 0; j < 64; ++j) {
		if ((texture->mask >> j) & 1) {
			((pixel*)texture->image.data)[j] = p;
		}
	}
	UnloadTexture(texture->texture);
	texture->texture = LoadTextureFromImage(texture->image);
}

void pallet_texture_free(pallet_texture* store) {
	UnloadImage(store->image);
	UnloadTexture(store->texture);
}
