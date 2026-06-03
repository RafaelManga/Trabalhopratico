CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c tabela.c lexico.c parser.c ast.c erros.c \
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
	rm -rf saidas/
	@echo "Limpeza concluida."

run: $(TARGET)
	./$(TARGET) ok1.pas
