CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,build/obj/%.o,$(SRC))

TARGET = build/philia

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

build/obj/%.o: src/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
