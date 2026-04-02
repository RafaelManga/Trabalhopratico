#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexico.h"
#include "palavraChave/palavraChave.h"
#include "numeros/numeros.h"
#include "operacoes/operacoes.h"
#include "simbolos/simbolos.h"

/* Posição atual no arquivo */
static int linha  = 1;
static int coluna = 0;

/* Lê um caractere e atualiza linha/coluna */
static int lerChar(FILE *f) {
    int c = fgetc(f);
    if (c == '\n') {
        linha++;
        coluna = 0;
    } else if (c != EOF) {
        coluna++;
    }
    return c;
}

/*
 * Função principal: retorna o próximo token do arquivo.
 * Chama cada módulo dependendo do caractere encontrado.
 */
Token proximoToken(FILE *fonte, TabelaSimbolos *ts) {
    Token tok;
    int c;

    /* Pula espaços, tabs, quebras de linha e comentários */
    while (1) {
        c = lerChar(fonte);

        if (c == EOF) {
            tok.tipo   = TOKEN_EOF;
            tok.linha  = linha;
            tok.coluna = coluna;
            strncpy(tok.lexema, "EOF", MAX_LEXEMA - 1);
            return tok;
        }

        /* Ignora espaços em branco */
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            continue;

        /* Ignora comentários { ... } */
        if (c == '{') {
            while ((c = lerChar(fonte)) != '}' && c != EOF);
            continue;
        }

        break;
    }

    /* Guarda onde o token começa */
    int linhaToken  = linha;
    int colunaToken = coluna;

    /* --- PALAVRA (identificador ou palavra reservada) --- */
    if (isalpha((unsigned char)c)) {
        char buffer[MAX_LEXEMA];
        int j = 0;
        buffer[j++] = (char)tolower((unsigned char)c);

        /* Lê o resto da palavra */
        int cc;
        while ((cc = lerChar(fonte)) != EOF && (isalpha((unsigned char)cc) || isdigit((unsigned char)cc))) {
            if (j < MAX_LEXEMA - 1)
                buffer[j++] = (char)tolower((unsigned char)cc);
        }
        buffer[j] = '\0';

        /* Devolve o caractere que não faz parte da palavra */
        if (cc != EOF) ungetc(cc, fonte);

        strncpy(tok.lexema, buffer, MAX_LEXEMA - 1);
        tok.linha  = linhaToken;
        tok.coluna = colunaToken;

        /* Verifica se é palavra reservada ou identificador */
        if      (strcmp(buffer, "program") == 0) tok.tipo = KW_PROGRAM;
        else if (strcmp(buffer, "var")     == 0) tok.tipo = KW_VAR;
        else if (strcmp(buffer, "integer") == 0) tok.tipo = KW_INTEGER;
        else if (strcmp(buffer, "real")    == 0) tok.tipo = KW_REAL;
        else if (strcmp(buffer, "begin")   == 0) tok.tipo = KW_BEGIN;
        else if (strcmp(buffer, "end")     == 0) tok.tipo = KW_END;
        else if (strcmp(buffer, "if")      == 0) tok.tipo = KW_IF;
        else if (strcmp(buffer, "then")    == 0) tok.tipo = KW_THEN;
        else if (strcmp(buffer, "else")    == 0) tok.tipo = KW_ELSE;
        else if (strcmp(buffer, "while")   == 0) tok.tipo = KW_WHILE;
        else if (strcmp(buffer, "do")      == 0) tok.tipo = KW_DO;
        else {
            tok.tipo = ID;
            ts_inserir(ts, buffer, ID);
        }

        return tok;
    }

    /* --- NÚMERO (inteiro ou real) --- */
    if (isdigit((unsigned char)c)) {
        char buffer[MAX_LEXEMA];
        int j = 0;
        buffer[j++] = (char)c;

        int cc;
        /* Lê a parte inteira */
        while ((cc = lerChar(fonte)) != EOF && isdigit((unsigned char)cc)) {
            if (j < MAX_LEXEMA - 1) buffer[j++] = (char)cc;
        }

        /* Verifica se tem ponto decimal (NUM_REAL) */
        if (cc == '.') {
            int prox = lerChar(fonte);
            if (isdigit((unsigned char)prox)) {
                buffer[j++] = '.';
                buffer[j++] = (char)prox;
                while ((cc = lerChar(fonte)) != EOF && isdigit((unsigned char)cc)) {
                    if (j < MAX_LEXEMA - 1) buffer[j++] = (char)cc;
                }
                buffer[j] = '\0';
                if (cc != EOF) ungetc(cc, fonte);
                tok.tipo = NUM_REAL;
            } else {
                /* Ponto sem dígito depois — devolve os dois */
                if (prox != EOF) ungetc(prox, fonte);
                ungetc('.', fonte);
                buffer[j] = '\0';
                tok.tipo = NUM_INT;
            }
        } else {
            buffer[j] = '\0';
            if (cc != EOF) ungetc(cc, fonte);
            tok.tipo = NUM_INT;
        }

        strncpy(tok.lexema, buffer, MAX_LEXEMA - 1);
        tok.linha  = linhaToken;
        tok.coluna = colunaToken;
        return tok;
    }

    /* --- OPERADORES --- */
    if (c == '+') { tok.tipo = OP_AD;  strncpy(tok.lexema, "+", MAX_LEXEMA-1); }
    else if (c == '-') { tok.tipo = OP_MIN; strncpy(tok.lexema, "-", MAX_LEXEMA-1); }
    else if (c == '*') { tok.tipo = OP_MUL; strncpy(tok.lexema, "*", MAX_LEXEMA-1); }
    else if (c == '/') { tok.tipo = OP_DIV; strncpy(tok.lexema, "/", MAX_LEXEMA-1); }
    else if (c == '=') { tok.tipo = OP_EQ;  strncpy(tok.lexema, "=", MAX_LEXEMA-1); }

    else if (c == '<') {
        int prox = lerChar(fonte);
        if (prox == '>') { tok.tipo = OP_NE; strncpy(tok.lexema, "<>", MAX_LEXEMA-1); }
        else if (prox == '=') { tok.tipo = OP_LE; strncpy(tok.lexema, "<=", MAX_LEXEMA-1); }
        else { if (prox != EOF) ungetc(prox, fonte); tok.tipo = OP_LT; strncpy(tok.lexema, "<", MAX_LEXEMA-1); }
    }

    else if (c == '>') {
        int prox = lerChar(fonte);
        if (prox == '=') { tok.tipo = OP_GE; strncpy(tok.lexema, ">=", MAX_LEXEMA-1); }
        else { if (prox != EOF) ungetc(prox, fonte); tok.tipo = OP_GT; strncpy(tok.lexema, ">", MAX_LEXEMA-1); }
    }

    else if (c == ':') {
        int prox = lerChar(fonte);
        if (prox == '=') { tok.tipo = OP_ASS; strncpy(tok.lexema, ":=", MAX_LEXEMA-1); }
        else { if (prox != EOF) ungetc(prox, fonte); tok.tipo = SMB_COL; strncpy(tok.lexema, ":", MAX_LEXEMA-1); }
    }

    /* --- SÍMBOLOS --- */
    else if (c == ';') { tok.tipo = SMB_SEM; strncpy(tok.lexema, ";", MAX_LEXEMA-1); }
    else if (c == ',') { tok.tipo = SMB_COM; strncpy(tok.lexema, ",", MAX_LEXEMA-1); }
    else if (c == '(') { tok.tipo = SMB_OPA; strncpy(tok.lexema, "(", MAX_LEXEMA-1); }
    else if (c == ')') { tok.tipo = SMB_CPA; strncpy(tok.lexema, ")", MAX_LEXEMA-1); }
    else if (c == '.') { tok.tipo = SMB_DOT; strncpy(tok.lexema, ".", MAX_LEXEMA-1); }
    else if (c == '}') { tok.tipo = SMB_CBC; strncpy(tok.lexema, "}", MAX_LEXEMA-1); }

    /* --- CARACTERE INVÁLIDO --- */
    else {
        tok.tipo      = TOKEN_ERRO;
        tok.lexema[0] = (char)c;
        tok.lexema[1] = '\0';
    }

    tok.linha  = linhaToken;
    tok.coluna = colunaToken;
    return tok;
}