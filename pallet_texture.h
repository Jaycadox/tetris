#pragma once
#include <stdint.h>
#include <raylib.h>
#include <stdio.h>

typedef struct {
	Image image;
	size_t size;
	size_t mask;
	Texture2D texture;
} pallet_texture;

pallet_texture pallet_texture_new(Image img);
void pallet_texture_apply_pallet(pallet_texture* texture, uint32_t colour);
void pallet_texture_free(pallet_texture* store);
