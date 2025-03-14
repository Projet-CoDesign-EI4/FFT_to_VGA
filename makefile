CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = test
SOURCES = image_formatter.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
