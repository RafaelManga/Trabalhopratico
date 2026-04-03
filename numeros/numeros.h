#ifndef NUMEROS_H
#define NUMEROS_H

#include "../token.h"

/* Lê um número (inteiro ou real) a partir da posição i e retorna o token.
   NUM_INT: apenas dígitos (ex: 42)
   NUM_REAL: dígitos + ponto + dígitos (ex: 3.14) */
Token lerNumero(const char *str, int i, int linha, int *colFinal);

#endif
