#include <stdio.h>
#include <string.h>
#include "operacoes.h"

Token lerOperador(const char *str, int i, int linha, int *colFinal) {
    Token tok;
    tok.linha  = linha;
    tok.coluna = i + 1;
    *colFinal  = i;

    switch (str[i]) {
        case '+':
            tok.tipo = OP_AD;
            strncpy(tok.lexema, "+", MAX_LEXEMA - 1);
            break;

        case '-':
            tok.tipo = OP_MIN;
            strncpy(tok.lexema, "-", MAX_LEXEMA - 1);
            break;

        case '*':
            tok.tipo = OP_MUL;
            strncpy(tok.lexema, "*", MAX_LEXEMA - 1);
            break;

        case '/':
            tok.tipo = OP_DIV;
            strncpy(tok.lexema, "/", MAX_LEXEMA - 1);
            break;

        case '=':
            tok.tipo = OP_EQ;
            strncpy(tok.lexema, "=", MAX_LEXEMA - 1);
            break;

        case '>':
            if (str[i + 1] == '=') {
                tok.tipo = OP_GE;
                strncpy(tok.lexema, ">=", MAX_LEXEMA - 1);
                *colFinal = i + 1;
            } else {
                tok.tipo = OP_GT;
                strncpy(tok.lexema, ">", MAX_LEXEMA - 1);
            }
            break;

        case '<':
            if (str[i + 1] == '>') {
                tok.tipo = OP_NE;
                strncpy(tok.lexema, "<>", MAX_LEXEMA - 1);
                *colFinal = i + 1;
            } else if (str[i + 1] == '=') {
                tok.tipo = OP_LE;
                strncpy(tok.lexema, "<=", MAX_LEXEMA - 1);
                *colFinal = i + 1;
            } else {
                tok.tipo = OP_LT;
                strncpy(tok.lexema, "<", MAX_LEXEMA - 1);
            }
            break;

        case ':':
            if (str[i + 1] == '=') {
                tok.tipo = OP_ASS;
                strncpy(tok.lexema, ":=", MAX_LEXEMA - 1);
                *colFinal = i + 1;
            } else {
                /* Só ':' sozinho é símbolo SMB_COL, não operador */
                tok.tipo = SMB_COL;
                strncpy(tok.lexema, ":", MAX_LEXEMA - 1);
            }
            break;

        default:
            tok.tipo = TOKEN_ERRO;
            tok.lexema[0] = str[i];
            tok.lexema[1] = '\0';
            break;
    }

    return tok;
}
