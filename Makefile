CC = gcc
CFLAGS = -Wall -Werror -pedantic
SRCS = src/main.c src/escalonador.c src/processo.c src/fila.c src/interface.c src/auxiliar.c
OBJ = main

.PHONY: all clean

all: $(OBJ)
	./$(OBJ)

$(OBJ): $(SRCS)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRCS)

clean:
	rm -f $(OBJ)

