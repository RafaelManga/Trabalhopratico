#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "../token.h"

/* Lê um símbolo de pontuação e retorna o token correspondente */
Token lerSimbolo(const char *str, int i, int linha);

#endif
