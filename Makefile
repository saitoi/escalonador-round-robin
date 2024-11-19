CC = gcc
CFLAGS = -Wall -Werror -pedantic
SRCS_COMMON = src/main.c src/escalonador.c src/processo.c src/fila.c src/utilitarios.c
SRCS_DEFAULT = $(SRCS_COMMON) src/interface.c
SRCS_PRETTY = $(SRCS_COMMON) src/interface-pretty.c
TARGET = main
TARGET_PRETTY = main-pretty

.PHONY: all pretty clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS_DEFAULT)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS_DEFAULT)

pretty: $(TARGET_PRETTY)
	./$(TARGET_PRETTY)

$(TARGET_PRETTY): $(SRCS_PRETTY)
	$(CC) $(CFLAGS) -o $(TARGET_PRETTY) $(SRCS_PRETTY)

clean:
	rm -f $(TARGET) $(TARGET_PRETTY)
