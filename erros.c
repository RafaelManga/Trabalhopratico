#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "erros.h"

#define MAX_ERROS 256

/* Estrutura de um erro léxico */
typedef struct {
    char tipo[64];
    char detalhe[256];
    int linha;
    int coluna;
} Erro;

static Erro listaErros[MAX_ERROS];
static int totalErros = 0;

/* Registra um erro na lista.
   Chamada automaticamente pelo lexico.c quando acha um caractere inválido
   ou pelo main.c ao detectar comentário não fechado. */
void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe) {
    if (totalErros >= MAX_ERROS) return;

    strncpy(listaErros[totalErros].tipo,    tipo,    63);
    strncpy(listaErros[totalErros].detalhe, detalhe, 255);
    listaErros[totalErros].linha  = linha;
    listaErros[totalErros].coluna = coluna;
    totalErros++;

    /* Mostra o erro no terminal também */
    fprintf(stderr, "ERRO LEXICO [linha %d, col %d] %s: %s\n",
            linha, coluna, tipo, detalhe);
}

/* Retorna quantos erros foram encontrados */
int obterTotalErros() {
    return totalErros;
}

/* Salva todos os erros no arquivo .err */
void salvarErros(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) {
        fprintf(stderr, "Erro ao criar arquivo %s\n", nomeArquivo);
        return;
    }

    if (totalErros == 0) {
        fprintf(f, "Nenhum erro lexico encontrado.\n");
    } else {
        fprintf(f, "%-25s | %-6s | %-6s | %s\n", "Tipo", "Linha", "Coluna", "Detalhe");
        fprintf(f, "----------------------------------------------------------\n");
        for (int i = 0; i < totalErros; i++) {
            fprintf(f, "%-25s | %-6d | %-6d | %s\n",
                listaErros[i].tipo,
                listaErros[i].linha,
                listaErros[i].coluna,
                listaErros[i].detalhe);
        }
        fprintf(f, "----------------------------------------------------------\n");
        fprintf(f, "Total: %d erro(s) encontrado(s).\n", totalErros);
    }

    fclose(f);
    printf("Erros salvos em: %s\n", nomeArquivo);
}
