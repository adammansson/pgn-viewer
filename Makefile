TARGET := pgn-viewer

SOURCE_DIR := src
SRCS := $(wildcard $(SOURCE_DIR)/*/*.c $(SOURCE_DIR)/*.c)
HDRS := $(wildcard $(SOURCE_DIR)/*/*.h $(SOURCE_DIR)/*.h)
OBJS := ${SRCS:.c=.o}

CC := cc
CFLAGS := -Wall -Wextra -std=c11 -pedantic

$(TARGET): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

run: $(TARGET)
	./$(TARGET)
