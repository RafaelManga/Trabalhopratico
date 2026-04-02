#ifndef PALAVRACHAVE_H
#define PALAVRACHAVE_H

#include "../token.h"
#include "../tabela.h"

/* Lê uma palavra do código a partir da posição i e retorna o token correspondente.
   Pode ser uma palavra reservada (if, while, begin...) ou um identificador (nome de variável). */
Token lerPalavra(const char *str, int i, int linha, int *colFinal, TabelaSimbolos *ts);

#endif
