#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "tabela.h"

Token proximoToken(FILE *fonte, TabelaSimbolos *ts);
void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe);
void salvarErros(const char *nomeArquivo);

static void montarNome(const char *entrada, const char *ext, char *saida) {
    char base[512];
    strncpy(base, entrada, sizeof(base) - 1);
    char *ponto = strrchr(base, '.');
    if (ponto) *ponto = '\0';
    snprintf(saida, 512, "%s.%s", base, ext);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    FILE *fonte = fopen(argv[1], "r");
    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    char arquivoLex[512], arquivoTs[512], arquivoErr[512];
    montarNome(argv[1], "lex", arquivoLex);
    montarNome(argv[1], "ts",  arquivoTs);
    montarNome(argv[1], "err", arquivoErr);

    TabelaSimbolos ts;
    ts_inicializar(&ts);

    FILE *fLex = fopen(arquivoLex, "w");
    if (!fLex) { fclose(fonte); return 1; }

    printf("Analisando: %s\n", argv[1]);

    Token tok;
    do {
        tok = proximoToken(fonte, &ts);
        if (tok.tipo == TOKEN_ERRO) continue;
        if (tok.tipo == TOKEN_EOF) break;
        fprintf(fLex, "<%s, %s> %d %d\n",
            nomeToken(tok.tipo), tok.lexema, tok.linha, tok.coluna);
    } while (tok.tipo != TOKEN_EOF);

    fclose(fonte);
    fclose(fLex);

    ts_salvar(&ts, arquivoTs);
    salvarErros(arquivoErr);

    printf("Saida:\n  %s\n  %s\n  %s\n", arquivoLex, arquivoTs, arquivoErr);
    ts_imprimir(&ts);
    return 0;
}
