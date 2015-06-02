PROGRAM_NAME := doppel
LIBRARY_NAME := libgame.so

LIBRARY_PATH := -L/usr/local/include
LIBRARIES := SDL2 SDL2_image
LD_FLAGS += $(foreach library, $(LIBRARIES),-l$(library))

CC := gcc
STD_FLAGS := -std=gnu99

SIMPLESTRICT = -Werror -Wall

SUPERSTRICT = -Werror -Wall -Wextra -pedantic-errors -Wformat=2 -Wno-import \
		   -Wimplicit -Wmain -Wchar-subscripts -Wsequence-point -Wmissing-braces \
		   -Wparentheses -Winit-self -Wswitch-enum -Wstrict-aliasing=2 -Wundef \
		   -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align \
		   -Wwrite-strings -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes \
		   -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline \
		   -Wdisabled-optimization -Wunused-macros -Wno-unused

WARNINGS = $(SIMPLESTRICT)

LFILES := game texture timer animation keyboard sprite
LIB_FILES += $(foreach file, $(LFILES), source/$(file).c)
LIB_O_FILES += $(foreach file, $(LFILES), $(file).o)

MFILES := main.c keyboard.c
MAIN_FILES += $(foreach file, $(MFILES), source/$(file))

libgame:
	#libgame built
	($(CC) -c $(WARNINGS) $(STD_FLAGS) -fpic $(LIB_FILES))
	($(CC) -shared -o $(LIBRARY_NAME) $(LIB_O_FILES) $(LIBRARY_PATH) $(LD_FLAGS))

main:
	# this will build the main application.
	$(CC) $(WARNINGS)  $(STD_FLAGS) $(MAIN_FILES) -o $(PROGRAM_NAME) $(LIBRARY_PATH) $(LD_FLAGS)

clean:
	rm *.o *.so doppel

fswatch:
	# this will only work on osx, for linux run the separate notify.sh
	fswatch-run ./source "make libgame"

all:libgame main
