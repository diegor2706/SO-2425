# Makefile for compiling p0.c into an executable named p0

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Target executable name
TARGET = p0

# Source files
SRCS = p0.c

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
