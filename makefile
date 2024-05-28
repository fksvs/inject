CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = inject

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY = clean
clean:
	rm -f $(TARGET) $(OBJECTS)
