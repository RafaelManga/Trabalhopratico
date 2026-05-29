#ifndef TABELA_H
#define TABELA_H

#include "token.h"

/* Limite máximo de entradas na tabela */
#define MAX_ENTRADAS 1024

/*
 * Uma linha da tabela de símbolos.
 * Guarda o nome (lexema) e o tipo daquele símbolo.
 */
typedef struct {
    char lexema[MAX_LEXEMA];
    TipoToken tipo;
} EntradaTS;

/*
 * A tabela de símbolos em si.
 * É basicamente um vetor de entradas com um contador de quantas existem.
 */
typedef struct {
    EntradaTS entradas[MAX_ENTRADAS];
    int tamanho;
} TabelaSimbolos;

/* Preenche a tabela com todas as palavras reservadas da linguagem */
void ts_inicializar(TabelaSimbolos *ts);

/* Procura um lexema na tabela. Retorna a posição ou -1 se não achar */
int  ts_buscar(TabelaSimbolos *ts, const char *lexema);

/* Insere um lexema se ele ainda não estiver na tabela. Retorna a posição */
int  ts_inserir(TabelaSimbolos *ts, const char *lexema, TipoToken tipo);

/* Salva a tabela inteira no arquivo .ts */
void ts_salvar(TabelaSimbolos *ts, const char *nomeArquivo);

/* Mostra a tabela no terminal (útil para debugar) */
void ts_imprimir(TabelaSimbolos *ts);

#endif
