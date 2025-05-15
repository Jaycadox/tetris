SOURCES := board.c main.c score.c tetromino.c texture_store.c pallet_texture.c
main: main.c
	clang -o Tetris $(SOURCES) -lraylib -I/usr/include/raylib -lm -lgbm -ldrm -lEGL -lGL -std=c99
