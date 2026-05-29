#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "numeros.h"

Token lerNumero(const char *str, int i, int linha, int *colFinal) {
    Token tok;
    int j = i;
    int isReal = 0;
    int k = 0;
    char buffer[MAX_LEXEMA];

    tok.linha  = linha;
    tok.coluna = i + 1;

    /* Lê a parte inteira */
    while (isdigit(str[j])) {
        buffer[k++] = str[j++];
    }

    /* Verifica se tem ponto decimal (parte obrigatória para NUM_REAL) */
    if (str[j] == '.' && isdigit(str[j + 1])) {
        isReal = 1;
        buffer[k++] = str[j++]; /* copia o ponto */

        while (isdigit(str[j])) {
            buffer[k++] = str[j++];
        }
    }

    buffer[k] = '\0';
    *colFinal = j - 1;

    strncpy(tok.lexema, buffer, MAX_LEXEMA - 1);
    tok.tipo = isReal ? NUM_REAL : NUM_INT;

    return tok;
}
