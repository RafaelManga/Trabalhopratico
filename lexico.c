#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexico.h"
#include "erros.h"
#include "palavraChave/palavraChave.h"
#include "numeros/numeros.h"
#include "operacoes/operacoes.h"
#include "simbolos/simbolos.h"

static int linha  = 1;
static int coluna = 0;


void lexico_resetar_linha(void) {
    linha  = 1;
    coluna = 0;
}

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


static void devolverChar(int c, FILE *f) {
    if (c == EOF) return;
    if (c == '\n') {
        linha--;
        coluna = 0; 
    } else {
        coluna--;
    }
    ungetc(c, f);
}


Token proximoToken(FILE *fonte, TabelaSimbolos *ts) {
    Token tok;
    int c;

    while (1) {
        c = lerChar(fonte);

        if (c == EOF) {
            tok.tipo   = TOKEN_EOF;
            tok.linha  = linha;
            tok.coluna = coluna;
            strncpy(tok.lexema, "EOF", MAX_LEXEMA - 1);
            return tok;
        }

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            continue;

        if (c == '{') {
            int linComent = linha;
            int colComent = coluna;
            int fechou = 0;
            while ((c = lerChar(fonte)) != EOF) {
                if (c == '}') { fechou = 1; break; }
            }
            if (!fechou) {
                registrarErro("COMENTARIO_NAO_FECHADO", linComent, colComent,
                              "Fim de arquivo atingido sem fechar o comentario '}'");
                tok.tipo   = TOKEN_EOF;
                tok.linha  = linha;
                tok.coluna = coluna;
                strncpy(tok.lexema, "EOF", MAX_LEXEMA - 1);
                return tok;
            }
            continue;
        }

        break;
    }

    int linhaToken  = linha;
    int colunaToken = coluna;

    if (isalpha((unsigned char)c)) {
        char buffer[MAX_LEXEMA];
        int j = 0;
        buffer[j++] = (char)tolower((unsigned char)c);

        int cc;
        while ((cc = lerChar(fonte)) != EOF && (isalpha((unsigned char)cc) || isdigit((unsigned char)cc))) {
            if (j < MAX_LEXEMA - 1)
                buffer[j++] = (char)tolower((unsigned char)cc);
        }
        buffer[j] = '\0';

        devolverChar(cc, fonte);

        strncpy(tok.lexema, buffer, MAX_LEXEMA - 1);
        tok.linha  = linhaToken;
        tok.coluna = colunaToken;

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

    if (isdigit((unsigned char)c)) {
        char buffer[MAX_LEXEMA];
        int j = 0;
        buffer[j++] = (char)c;

        int cc;
        while ((cc = lerChar(fonte)) != EOF && isdigit((unsigned char)cc)) {
            if (j < MAX_LEXEMA - 1) buffer[j++] = (char)cc;
        }

        if (cc == '.') {
            int prox = lerChar(fonte);
            if (isdigit((unsigned char)prox)) {
                buffer[j++] = '.';
                buffer[j++] = (char)prox;
                while ((cc = lerChar(fonte)) != EOF && isdigit((unsigned char)cc)) {
                    if (j < MAX_LEXEMA - 1) buffer[j++] = (char)cc;
                }
                buffer[j] = '\0';
                if (cc != EOF) devolverChar(cc, fonte);
                tok.tipo = NUM_REAL;
            } else {
                if (prox != EOF) devolverChar(prox, fonte);
                devolverChar('.', fonte);
                buffer[j] = '\0';
                tok.tipo = NUM_INT;
            }
        } else {
            buffer[j] = '\0';
            if (cc != EOF) devolverChar(cc, fonte);
            tok.tipo = NUM_INT;
        }

        strncpy(tok.lexema, buffer, MAX_LEXEMA - 1);
        tok.linha  = linhaToken;
        tok.coluna = colunaToken;
        return tok;
    }

    if (c == '+') { tok.tipo = OP_AD;  strncpy(tok.lexema, "+",  MAX_LEXEMA-1); }
    else if (c == '-') { tok.tipo = OP_MIN; strncpy(tok.lexema, "-",  MAX_LEXEMA-1); }
    else if (c == '*') { tok.tipo = OP_MUL; strncpy(tok.lexema, "*",  MAX_LEXEMA-1); }
    else if (c == '/') { tok.tipo = OP_DIV; strncpy(tok.lexema, "/",  MAX_LEXEMA-1); }
    else if (c == '=') { tok.tipo = OP_EQ;  strncpy(tok.lexema, "=",  MAX_LEXEMA-1); }

    else if (c == '<') {
        int prox = lerChar(fonte);
        if      (prox == '>') { tok.tipo = OP_NE; strncpy(tok.lexema, "<>", MAX_LEXEMA-1); }
        else if (prox == '=') { tok.tipo = OP_LE; strncpy(tok.lexema, "<=", MAX_LEXEMA-1); }
        else { if (prox != EOF) devolverChar(prox, fonte); tok.tipo = OP_LT; strncpy(tok.lexema, "<", MAX_LEXEMA-1); }
    }

    else if (c == '>') {
        int prox = lerChar(fonte);
        if (prox == '=') { tok.tipo = OP_GE; strncpy(tok.lexema, ">=", MAX_LEXEMA-1); }
        else { if (prox != EOF) devolverChar(prox, fonte); tok.tipo = OP_GT; strncpy(tok.lexema, ">", MAX_LEXEMA-1); }
    }

    else if (c == ':') {
        int prox = lerChar(fonte);
        if (prox == '=') { tok.tipo = OP_ASS; strncpy(tok.lexema, ":=", MAX_LEXEMA-1); }
        else { if (prox != EOF) devolverChar(prox, fonte); tok.tipo = SMB_COL; strncpy(tok.lexema, ":", MAX_LEXEMA-1); }
    }

    else if (c == ';') { tok.tipo = SMB_SEM; strncpy(tok.lexema, ";", MAX_LEXEMA-1); }
    else if (c == ',') { tok.tipo = SMB_COM; strncpy(tok.lexema, ",", MAX_LEXEMA-1); }
    else if (c == '(') { tok.tipo = SMB_OPA; strncpy(tok.lexema, "(", MAX_LEXEMA-1); }
    else if (c == ')') { tok.tipo = SMB_CPA; strncpy(tok.lexema, ")", MAX_LEXEMA-1); }
    else if (c == '.') { tok.tipo = SMB_DOT; strncpy(tok.lexema, ".", MAX_LEXEMA-1); }
    else if (c == '}') { tok.tipo = SMB_CBC; strncpy(tok.lexema, "}", MAX_LEXEMA-1); }

    else {
        char detalhe[50];
        sprintf(detalhe, "Simbolo '%c' nao reconhecido", c);
        registrarErro("CARACTER_INVALIDO", linhaToken, colunaToken, detalhe);
        tok.tipo      = TOKEN_ERRO;
        tok.lexema[0] = (char)c;
        tok.lexema[1] = '\0';
    }

    tok.linha  = linhaToken;
    tok.coluna = colunaToken;
    return tok;
}
