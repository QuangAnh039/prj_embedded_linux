# Compiler and flags
CC = gcc
CFLAGS = -pthread -Wall -ggdb3
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# Target executable
TARGET = peers

# Source and include directories
SRC = src
INC = include

# Source and object files
SRCS = $(SRC)/main.c $(SRC)/my_feature.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Compile object files
$(SRC)/%.o: $(SRC)/%.c $(INC)/my_feature.h
	$(CC) -c -o $@ $< $(CFLAGS)

# Run valgrind
valgrind: $(TARGET)
	$(VALGRIND) ./$(TARGET)

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
