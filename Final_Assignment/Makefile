# Makefile for Sensor Gateway

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -pthread -lsqlite3

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/connmgr.c \
       $(SRC_DIR)/datamgr.c \
       $(SRC_DIR)/storagemgr.c \
       $(SRC_DIR)/sbuffer.c \
       $(SRC_DIR)/log_util.c

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET = sensor_gateway

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run with Valgrind memory check
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) 1234

# Run the program
run: $(TARGET)
	./$(TARGET) 1234

# Debug target
debug: CFLAGS += -DDEBUG
debug: all

.PHONY: all clean memcheck run debug