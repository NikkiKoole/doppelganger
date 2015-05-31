PROGRAM_NAME := doppel
LIBRARY_NAME := libgame.so

LIBRARY_PATH := -L/usr/local/include
LIBRARIES := SDL2 SDL2_image
LD_FLAGS += $(foreach library, $(LIBRARIES),-l$(library))

CC := gcc
STD_FLAGS := -std=gnu99
COMPILER_FLAGS :=  -Wall -Werror

LFILES := game texture timer animation sprite keyboard
LIB_FILES += $(foreach file, $(LFILES), source/$(file).c)
LIB_O_FILES += $(foreach file, $(LFILES), $(file).o)

MFILES := main.c keyboard.c
MAIN_FILES += $(foreach file, $(MFILES), source/$(file))

libgame:
	#libgame built
	@($(CC) -c $(COMPILER_FLAGS) $(STD_FLAGS) -fpic $(LIB_FILES))
	@($(CC) -shared -o $(LIBRARY_NAME) $(LIB_O_FILES) $(LIBRARY_PATH) $(LD_FLAGS))

main:
	# this will build the main application.
	$(CC) $(COMPILER_FLAGS)  $(STD_FLAGS) $(MAIN_FILES) -o $(PROGRAM_NAME) $(LIBRARY_PATH) $(LD_FLAGS)

clean:
	rm *.o *.so doppel

fswatch:
	# this will only work on osx, for linux run the separate notify.sh
	fswatch-run ./source "make libgame"

all:libgame main
