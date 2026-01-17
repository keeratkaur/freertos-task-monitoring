# FreeRTOS Task Monitoring - Makefile
# 
# This Makefile is designed to work with FreeRTOS POSIX simulator
# For actual hardware targets, modify paths and compiler settings accordingly

CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lpthread -lrt

# FreeRTOS paths (adjust these to match your FreeRTOS installation)
FREERTOS_DIR = ./FreeRTOS
FREERTOS_KERNEL_DIR = $(FREERTOS_DIR)/Source
FREERTOS_PORT_DIR = $(FREERTOS_KERNEL_DIR)/portable/Posix

# Include directories
INCLUDES = -I. \
           -I$(FREERTOS_KERNEL_DIR)/include \
           -I$(FREERTOS_PORT_DIR)

# Source files
MAIN_SRC = main.c

# FreeRTOS core source files (minimal set for demo)
FREERTOS_SRCS = $(FREERTOS_KERNEL_DIR)/tasks.c \
                $(FREERTOS_KERNEL_DIR)/queue.c \
                $(FREERTOS_KERNEL_DIR)/list.c \
                $(FREERTOS_KERNEL_DIR)/timers.c \
                $(FREERTOS_PORT_DIR)/port.c

# Runtime stats helper
RUNTIME_STATS_SRC = runtime_stats.c

# Object files
OBJS = $(MAIN_SRC:.c=.o) $(FREERTOS_SRCS:.c=.o) $(RUNTIME_STATS_SRC:.c=.o)

# Target executable
TARGET = freertos_monitor

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile main.c
main.o: main.c FreeRTOSConfig.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c -o main.o

# Compile FreeRTOS source files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Clean complete"

# Run the application
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "FreeRTOS Task Monitoring - Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all     - Build the application (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  run     - Build and run the application"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Note: This Makefile assumes FreeRTOS is in ./FreeRTOS"
	@echo "      Adjust FREERTOS_DIR in Makefile if needed"

.PHONY: all clean run help

