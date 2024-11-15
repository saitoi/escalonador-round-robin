CC = gcc
CFLAGS = -Wall -Werror -pedantic
SRCS = src/main.c src/escalonador.c src/processo.c src/fila.c src/interface.c src/utilitarios.c
TARGET = main

.PHONY: all clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
