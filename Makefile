TARGET := pgn-viewer

SRCS_DIR := src
SRCS := $(wildcard $(SRCS_DIR)/*/*.c $(SRCS_DIR)/*.c)
HDRS := $(wildcard $(SRCS_DIR)/*/*.h $(SRCS_DIR)/*.h)
OBJS := ${SRCS:.c=.o}

CC := cc
CFLAGS := -Wall -Wextra -std=c11 -pedantic

$(TARGET): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM)	$(OBJS)
	$(RM)	$(TARGET)
