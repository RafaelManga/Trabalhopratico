#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>
#include "token.h"
#include "tabela.h"

Token proximoToken(FILE *fonte, TabelaSimbolos *ts);

// estou ficando louco, mas isso é necessário para resetar a contagem de linha entre a fase léxica e sintática
void lexico_resetar_linha(void);

#endif
