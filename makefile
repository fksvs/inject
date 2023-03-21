CC = gcc
CFLAGS = -Wall

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = inject

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo compile completed

.PHONY = clean
clean:
	@echo deleting object files and executable
	rm -f $(TARGET) $(OBJECTS)
	@echo deleted successfully
