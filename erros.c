#include <stdio.h>
#include <string.h>
#include "erros.h"

/* === ARQUIVO DA PESSOA 4 ===
   Implementação provisória para o projeto compilar.
   A pessoa 4 deve substituir esse arquivo pela versão completa. */

#define MAX_ERROS 256

typedef struct {
    char tipo[64];
    char detalhe[256];
    int linha;
    int coluna;
} Erro;

static Erro listaErros[MAX_ERROS];
static int totalErros = 0;

void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe) {
    if (totalErros >= MAX_ERROS) return;
    strncpy(listaErros[totalErros].tipo,   tipo,   63);
    strncpy(listaErros[totalErros].detalhe, detalhe, 255);
    listaErros[totalErros].linha  = linha;
    listaErros[totalErros].coluna = coluna;
    totalErros++;
    fprintf(stderr, "ERRO LEXICO [linha %d, col %d] %s: %s\n", linha, coluna, tipo, detalhe);
}

void salvarErros(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) return;
    if (totalErros == 0) {
        fprintf(f, "Nenhum erro lexico encontrado.\n");
    } else {
        fprintf(f, "%-25s | %-6s | %-6s | %s\n", "Tipo", "Linha", "Coluna", "Detalhe");
        fprintf(f, "-------------------------------------------------------\n");
        for (int i = 0; i < totalErros; i++)
            fprintf(f, "%-25s | %-6d | %-6d | %s\n",
                listaErros[i].tipo,
                listaErros[i].linha,
                listaErros[i].coluna,
                listaErros[i].detalhe);
    }
    fclose(f);
    printf("Erros salvos em: %s\n", nomeArquivo);
}
