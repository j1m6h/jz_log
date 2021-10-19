CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
LDFLAGS = -shared
TARGET = libjz_log.so

SRC = log.c user.c 
OBJ = ${SRC:.c=.o}

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
