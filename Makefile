# Define variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g -DUSE_ANSI
TARGET = cwordle
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean