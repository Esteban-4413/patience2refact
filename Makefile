CC = gcc
CFLAGS = -Wall -g

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    LDLIBS = -lncurses
else
    LDLIBS = -lncursesw
endif

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = jogo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf src/*.o $(TARGET) save.txt $(TARGET).dSYM Contents
