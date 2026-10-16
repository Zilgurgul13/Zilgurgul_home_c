CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = temperature_app.exe

OBJ = main.o temp_functions.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c temp_functions.h
	$(CC) $(CFLAGS) -c main.c -o main.o

temp_functions.o: temp_functions.c temp_functions.h
	$(CC) $(CFLAGS) -c temp_functions.c -o temp_functions.o
