CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c tabela.c lexico.c erros.c \
       palavraChave/palavraChave.c \
       numeros/numeros.c \
       operacoes/operacoes.c \
       simbolos/simbolos.c

OBJS = $(SRCS:.c=.o)
TARGET = analisador

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET) teste.pas
