
game:
	# this will build a library (to be loaded by main)
	gcc -c -Wall -Werror -fpic game.c
	gcc -shared -o libgame.so game.o

main:
	# this will build the main application.
	gcc main.c -o doppel -std=gnu99 -L/usr/local/include -lSDL2 -lSDL2_image

