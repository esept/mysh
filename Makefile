# Makefile for BomberStudentProject

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/exec

# Default target
all: $(TARGET)

# Link object files to create the target executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean