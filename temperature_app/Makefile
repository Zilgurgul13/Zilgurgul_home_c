CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = temperature_app

all: $(TARGET)

$(TARGET): main.o temp_api.o
	$(CC) $(CFLAGS) main.o temp_api.o -o $(TARGET)

main.o: main.c temp_api.h
	$(CC) $(CFLAGS) -c main.c

temp_api.o: temp_api.c temp_api.h
	$(CC) $(CFLAGS) -c temp_api.c

clean:
	rm -f $(TARGET) *.o
