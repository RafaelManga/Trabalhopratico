#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>
#include "token.h"
#include "tabela.h"

/* Função principal do analisador léxico.
   Lê o arquivo fonte caractere por caractere e retorna
   o próximo token encontrado a cada chamada. */
Token proximoToken(FILE *fonte, TabelaSimbolos *ts);

#endif
