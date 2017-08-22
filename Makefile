
CC ?= clang
CXX ?= clang++

CFLAGS  += -g -Wall -std=c11 
LDFLAGS += 
LDLIBS  += 

# SDL2
CFLAGS  += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs-only-L sdl2)
LDLIBS  += $(shell pkg-config --libs-only-l sdl2)

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))
OUT = bin/colormac

$(shell mkdir -p src/ obj/ bin/)

all: $(OUT)

run: $(OUT)
	./$(OUT)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

