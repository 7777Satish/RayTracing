# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -pthread -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Link the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile .c to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if not present
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run
