CC=gcc
CFLAGS= -std=c99 -Wall -Werror -Wpedantic -O3 -I./include
LDFLAGS= -lm -lSDL2
PROJECT_NAME=unknown
SOURCE_FILES=$(wildcard src/*.c)

build: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_NAME) main.c $(SOURCE_FILES)

clean:
	rm -f $(PROJECT_NAME)
