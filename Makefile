# Makefile for Multi-User Task Management System
# 
# This Makefile builds the task management system from modular C source files.
# 
# Usage:
#   make          - Build the project
#   make clean    - Remove build artifacts
#   make run      - Build and run the application
#   make debug    - Build with debug symbols

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS =
DEBUG_FLAGS = -g -DDEBUG

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Target executable
TARGET = task_manager

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Header files
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Run the application
run: all
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Install (optional)
install: all
	cp $(TARGET) /usr/local/bin/

# Help target
help:
	@echo "Multi-User Task Management System - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make run      - Build and run the application"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make help     - Show this help message"

.PHONY: all clean run debug install help
