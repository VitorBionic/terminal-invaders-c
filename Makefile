CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 \
        -fsanitize=address,undefined -fno-omit-frame-pointer

SRC=$(wildcard src/*.c)
TARGET=invaders

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
