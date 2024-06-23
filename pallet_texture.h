#pragma once
#include <stdint.h>
#include <raylib.h>
#include <stdio.h>

// A pallet_texture represents an item which contains a raw image who can have a pallet applied to it to become a texture with
// custom colours
typedef struct {
	Image image;
	size_t size;
	size_t mask; // Saves location of modifiable pixels
	Texture2D texture;
} pallet_texture;

// Create pallet_texture from an Image
// NOTE: this will automatically attempt to load a default pallet
pallet_texture pallet_texture_new(Image img);

// Applies a custom colour to the pallet_texture and modifies the `texture` field
void pallet_texture_apply_pallet(pallet_texture* texture, uint32_t colour);

// NOTE: Images use a shared memory buffer when copying, so be careful as this does free the image (potential double-free)
// 	You might want to just call UnloadTexture on store->texture
void pallet_texture_free(pallet_texture* store);
