CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lncurses -Iinclude

SRC = src/main.c src/chatbot.c src/files.c src/grammar.c src/stdphilia.c
OBJ = $(SRC:.c=.o)

TARGET = build/philia

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -rf build

run: $(TARGET)
	./$(TARGET)

re: clean all
