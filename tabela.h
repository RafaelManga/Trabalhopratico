#ifndef TABELA_H
#define TABELA_H

#include "token.h"

#define MAX_ENTRADAS 1024

typedef struct {
    char lexema[MAX_LEXEMA];
    TipoToken tipo;
} EntradaTS;

typedef struct {
    EntradaTS entradas[MAX_ENTRADAS];
    int tamanho;
} TabelaSimbolos;

void ts_inicializar(TabelaSimbolos *ts);
int  ts_buscar(TabelaSimbolos *ts, const char *lexema);
int  ts_inserir(TabelaSimbolos *ts, const char *lexema, TipoToken tipo);
void ts_salvar(TabelaSimbolos *ts, const char *nomeArquivo);
void ts_imprimir(TabelaSimbolos *ts);

#endif