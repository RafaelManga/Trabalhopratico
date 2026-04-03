#include <stdio.h>
#include <string.h>
#include "simbolos.h"

Token lerSimbolo(const char *str, int i, int linha) {
    Token tok;
    tok.linha  = linha;
    tok.coluna = i + 1;
    tok.lexema[1] = '\0';
    tok.lexema[0] = str[i];

    switch (str[i]) {
        case ';': tok.tipo = SMB_SEM; break;
        case ',': tok.tipo = SMB_COM; break;
        case '(': tok.tipo = SMB_OPA; break;
        case ')': tok.tipo = SMB_CPA; break;
        case '.': tok.tipo = SMB_DOT; break;
        case '{': tok.tipo = SMB_OBC; break;
        case '}': tok.tipo = SMB_CBC; break;
        default:
            tok.tipo = TOKEN_ERRO;
            break;
    }

    return tok;
}
