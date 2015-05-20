
game:
	# this will build a library (to be loaded by main)
	gcc -c -Wall -Werror -fpic source/game.c
	gcc -shared -o libgame.so game.o -L/usr/local/include -lSDL2 -lSDL2_image

main:
	# this will build the main application.
	gcc source/main.c -o doppel -std=gnu99 -L/usr/local/include -lSDL2 -lSDL2_image
