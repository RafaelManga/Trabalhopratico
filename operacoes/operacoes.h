#ifndef OPERACOES_H
#define OPERACOES_H

#include "../token.h"

/* Lê um operador a partir da posição i e retorna o token correspondente.
   Trata operadores de dois caracteres como <=, >=, <>, := */
Token lerOperador(const char *str, int i, int linha, int *colFinal);

#endif
