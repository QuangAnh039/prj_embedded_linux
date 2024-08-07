CC = gcc
CFLAGS = -pthread
TARGET = peers
SRC = src
INC = include

SRCS = $(SRC)/main.c $(SRC)/my_feature.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(SRC)/%.o: $(SRC)/%.c $(INC)/my_feature.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
