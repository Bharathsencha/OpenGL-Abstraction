# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iincludes
LDFLAGS = -lglfw -lGL -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

# Target executable
TARGET = myapp

# Source files
SOURCES = main.c $(SRC_DIR)/graphics.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/graphics.o

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile main.c from root directory
$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files from src directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/graphics.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Phony targets (not actual files)
.PHONY: all run clean