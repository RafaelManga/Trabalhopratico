#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>
#include "token.h"
#include "tabela.h"

/* Função principal do analisador léxico.
   Lê o arquivo fonte caractere por caractere e retorna
   o próximo token encontrado a cada chamada. */
Token proximoToken(FILE *fonte, TabelaSimbolos *ts);

/* Reseta o contador interno de linha/coluna.
   Deve ser chamada antes da fase sintática, pois o arquivo
   é reaberto do início e o contador precisa volcar a 1. */
void lexico_resetar_linha(void);

#endif
