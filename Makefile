# Makefile

CC := cc

RAYLIB_SRC_PATH := ./raylib/raylib-5.5/src/
RAYLIB_LIBRARY_PATH := $(RAYLIB_SRC_PATH)
RAYLIB_LIBRARIES := -l:libraylib.a -lm -ldl -lpthread

CFLAGS := -Wall -Wextra -ggdb
INCLUDE_PATHS := -I. -I$(RAYLIB_SRC_PATH)

LDFLAGS := -L$(RAYLIB_LIBRARY_PATH)
LDLIBS := $(RAYLIB_LIBRARIES)

SOURCES := game.c
EXECUTABLE := timber-chop

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS) $(LDLIBS)
