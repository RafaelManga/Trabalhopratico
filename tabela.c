#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabela.h"
#include "token.h"


static void toLowerStr(const char *src, char *dst) {
    int i = 0;
    while (src[i]) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}


const char* nomeToken(TipoToken tipo) {
    switch (tipo) {
        case KW_PROGRAM: return "KW_PROGRAM";
        case KW_VAR:     return "KW_VAR";
        case KW_INTEGER: return "KW_INTEGER";
        case KW_REAL:    return "KW_REAL";
        case KW_BEGIN:   return "KW_BEGIN";
        case KW_END:     return "KW_END";
        case KW_IF:      return "KW_IF";
        case KW_THEN:    return "KW_THEN";
        case KW_ELSE:    return "KW_ELSE";
        case KW_WHILE:   return "KW_WHILE";
        case KW_DO:      return "KW_DO";
        case ID:         return "ID";
        case NUM_INT:    return "NUM_INT";
        case NUM_REAL:   return "NUM_REAL";
        case OP_EQ:      return "OP_EQ";
        case OP_NE:      return "OP_NE";
        case OP_LT:      return "OP_LT";
        case OP_LE:      return "OP_LE";
        case OP_GT:      return "OP_GT";
        case OP_GE:      return "OP_GE";
        case OP_AD:      return "OP_AD";
        case OP_MIN:     return "OP_MIN";
        case OP_MUL:     return "OP_MUL";
        case OP_DIV:     return "OP_DIV";
        case OP_ASS:     return "OP_ASS";
        case SMB_SEM:    return "SMB_SEM";
        case SMB_COM:    return "SMB_COM";
        case SMB_OPA:    return "SMB_OPA";
        case SMB_CPA:    return "SMB_CPA";
        case SMB_COL:    return "SMB_COL";
        case SMB_DOT:    return "SMB_DOT";
        case SMB_OBC:    return "SMB_OBC";
        case SMB_CBC:    return "SMB_CBC";
        case TOKEN_ERRO: return "ERRO";
        case TOKEN_EOF:  return "EOF";
        default:         return "DESCONHECIDO";
    }
}


void ts_inicializar(TabelaSimbolos *ts) {
    ts->tamanho = 0;
    ts_inserir(ts, "program", KW_PROGRAM);
    ts_inserir(ts, "var",     KW_VAR);
    ts_inserir(ts, "integer", KW_INTEGER);
    ts_inserir(ts, "real",    KW_REAL);
    ts_inserir(ts, "begin",   KW_BEGIN);
    ts_inserir(ts, "end",     KW_END);
    ts_inserir(ts, "if",      KW_IF);
    ts_inserir(ts, "then",    KW_THEN);
    ts_inserir(ts, "else",    KW_ELSE);
    ts_inserir(ts, "while",   KW_WHILE);
    ts_inserir(ts, "do",      KW_DO);
}

int ts_buscar(TabelaSimbolos *ts, const char *lexema) {
    char lower[MAX_LEXEMA];
    toLowerStr(lexema, lower);

    for (int i = 0; i < ts->tamanho; i++) {
        if (strcmp(ts->entradas[i].lexema, lower) == 0)
            return i;
    }
    return -1;
}


int ts_inserir(TabelaSimbolos *ts, const char *lexema, TipoToken tipo) {
    char lower[MAX_LEXEMA];
    toLowerStr(lexema, lower);

    int idx = ts_buscar(ts, lower);
    if (idx != -1) return idx;

    if (ts->tamanho >= MAX_ENTRADAS) {
        fprintf(stderr, "Erro: Tabela de Simbolos cheia!\n");
        exit(1);
    }

    strncpy(ts->entradas[ts->tamanho].lexema, lower, MAX_LEXEMA - 1);
    ts->entradas[ts->tamanho].tipo = tipo;
    return ts->tamanho++;
}

/* Grava a tabela de símbolos no arquivo .ts ao final da análise */
void ts_salvar(TabelaSimbolos *ts, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) { fprintf(stderr, "Erro ao criar %s\n", nomeArquivo); return; }

    fprintf(f, "%-20s | %s\n", "Lexema", "Tipo");
    fprintf(f, "-------------------------------------\n");
    for (int i = 0; i < ts->tamanho; i++)
        fprintf(f, "%-20s | %s\n", ts->entradas[i].lexema, nomeToken(ts->entradas[i].tipo));

    fclose(f);
    printf("Tabela de Simbolos salva em: %s\n", nomeArquivo);
}

/* Mostra a tabela no terminal, útil para conferir durante os testes */
void ts_imprimir(TabelaSimbolos *ts) {
    printf("\n=== Tabela de Simbolos ===\n");
    printf("%-20s | %s\n", "Lexema", "Tipo");
    printf("-------------------------------------\n");
    for (int i = 0; i < ts->tamanho; i++)
        printf("%-20s | %s\n", ts->entradas[i].lexema, nomeToken(ts->entradas[i].tipo));
    printf("Total: %d entradas\n\n", ts->tamanho);
}
