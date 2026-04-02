#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "palavraChave.h"

Token lerPalavra(const char *str, int i, int linha, int *colFinal, TabelaSimbolos *ts) {
    Token tok;
    char buffer[MAX_LEXEMA];
    int j = 0;
    int colInicio = i;

    /* Lê letras e dígitos (identificadores podem ter dígitos depois da 1ª letra) */
    while (isalpha(str[i]) || isdigit(str[i])) {
        buffer[j++] = str[i++];
    }
    buffer[j] = '\0';
    *colFinal = i - 1;

    /* Converte para minúsculo para comparar (linguagem é case-insensitive) */
    char lower[MAX_LEXEMA];
    int k = 0;
    while (buffer[k]) {
        lower[k] = (char)tolower((unsigned char)buffer[k]);
        k++;
    }
    lower[k] = '\0';

    /* Preenche as informações de posição do token */
    tok.linha  = linha;
    tok.coluna = colInicio + 1;
    strncpy(tok.lexema, lower, MAX_LEXEMA - 1);

    /* Verifica se é palavra reservada ou identificador */
    if      (strcmp(lower, "program") == 0) tok.tipo = KW_PROGRAM;
    else if (strcmp(lower, "var")     == 0) tok.tipo = KW_VAR;
    else if (strcmp(lower, "integer") == 0) tok.tipo = KW_INTEGER;
    else if (strcmp(lower, "real")    == 0) tok.tipo = KW_REAL;
    else if (strcmp(lower, "begin")   == 0) tok.tipo = KW_BEGIN;
    else if (strcmp(lower, "end")     == 0) tok.tipo = KW_END;
    else if (strcmp(lower, "if")      == 0) tok.tipo = KW_IF;
    else if (strcmp(lower, "then")    == 0) tok.tipo = KW_THEN;
    else if (strcmp(lower, "else")    == 0) tok.tipo = KW_ELSE;
    else if (strcmp(lower, "while")   == 0) tok.tipo = KW_WHILE;
    else if (strcmp(lower, "do")      == 0) tok.tipo = KW_DO;
    else {
        /* É um identificador: insere na tabela de símbolos */
        tok.tipo = ID;
        ts_inserir(ts, lower, ID);
    }

    return tok;
}
