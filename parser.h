#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "tabela.h"

void analisarSintatico(FILE *fonte, TabelaSimbolos *ts, const char *arquivoEntrada);

#endif
