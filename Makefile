CC = gcc
CFLAGS = -Wall -Wextra
TARGET = processflow
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
		rm -f $(TARGET)

.PHONY: all clean