PROGRAM_NAME := doppel
LIBRARY_NAME := libgame.so

LIBRARY_PATH := -L/usr/local/include
LIBRARIES := SDL2 SDL2_image
LD_FLAGS += $(foreach library, $(LIBRARIES),-l$(library))


CC := gcc
STD_FLAGS := -std=gnu99
COMPILER_FLAGS :=  -Wall -Werror

LFILES := game.c tile.c
LIB_FILES += $(foreach file, $(LFILES), source/$(file))

MFILES := main.c
MAIN_FILES += $(foreach file, $(MFILES), source/$(file))


game:
	# this will build a library (to be loaded by main)
	$(CC) -c $(COMPILER_FLAGS) -fpic $(LIB_FILES)
	$(CC) -shared -o $(LIBRARY_NAME) game.o tile.o $(LIBRARY_PATH) $(LD_FLAGS)

main:
	# this will build the main application.
	$(CC) $(COMPILER_FLAGS)  $(STD_FLAGS) $(MAIN_FILES) -o $(PROGRAM_NAME) $(LIBRARY_PATH) $(LD_FLAGS)
clean:
	rm *.o *.so doppel

fswatch:
	fswatch-run ./source "make game"

all:game main
