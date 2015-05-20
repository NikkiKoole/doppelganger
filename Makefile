PROGRAM_NAME := doppel
LIBRARY_NAME := libgame.so

LIBRARY_PATH := -L/usr/local/include
LIBRARIES := SDL2 SDL2_image
LD_FLAGS += $(foreach library, $(LIBRARIES),-l$(library))


CC := gcc
MAIN_C_FLAGS := -std=gnu99

LFILES := game.c
LIB_FILES += $(foreach file, $(LFILES), source/$(file))

MFILES := main.c
MAIN_FILES += $(foreach file, $(MFILES), source/$(file))


game:
	# this will build a library (to be loaded by main)
	$(CC) -c -Wall -Werror -fpic $(LIB_FILES)
	$(CC) -shared -o $(LIBRARY_NAME) game.o $(LIBRARY_PATH) $(LD_FLAGS)

main:
	# this will build the main application.
	$(CC) $(MAIN_C_FLAGS) $(MAIN_FILES) -o $(PROGRAM_NAME) $(LIBRARY_PATH) $(LD_FLAGS)

fswatch:
	fswatch-run ./source "make game"
