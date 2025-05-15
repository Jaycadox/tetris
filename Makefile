SOURCES := board.c board.h main.c score.c score.h tetromino.c tetromino.h texture_store.c texture_store.h pallet_texture.c pallet_texture.h
main: main.c
	clang $(SOURCES) -lraylib -I/usr/include/raylib -lm -lgbm -ldrm -lEGL -lGL -std=c99
