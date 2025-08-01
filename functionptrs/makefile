CC = gcc
CFLAGS = -Wall -g
TARGET = hello

all: $(TARGET)

$(TARGET): simplify.c
	$(CC) $(CFLAGS) -o $(TARGET) simplify.c -lm

clean:
	rm -f $(TARGET)
