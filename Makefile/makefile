CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/server.c
OBJ = $(SRC:.c=.o)
TARGET = my-http-server

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

clean:
    rm -f $(OBJ) $(TARGET)

.PHONY: all clean