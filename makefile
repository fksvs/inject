CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic

# Debug build flags
DEBUG_FLAGS = -g -O0 -DDEBUG

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = inject

# Default target
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

# Install target
install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)

# Uninstall target
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

.PHONY: clean debug install uninstall
clean:
	rm -f $(TARGET) $(OBJECTS)
